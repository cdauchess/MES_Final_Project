/* 
    This code originates from the Getting started with Raspberry Pi Pico document
    https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
    CC BY-ND Raspberry Pi (Trading) Ltd
*/
#include "main.h"

//Hardware Definitions
const uint LED_PIN = 25; //Onboard RP2040 LED
const uint PIN_TX = 15; //Neopixel Pin
const uint BUTTON_PIN = 14; //GPIO For button input
const uint CLOSE_PIN = 19; //Relay open pin
const uint OPEN_PIN = 18; //Relay close pin

//Button ISR Flag(s)
uint buttonEdge = 0; //1 for rising, 2 for falling
absolute_time_t prevEdgeTime;

//Card: 33C2494



uint32_t timerFlag = 0;

bool timerCallback(repeating_timer_t *rt){
    timerFlag = 1;
    return 1;
}

void buttonISR(uint gpio, uint32_t events){
    if(gpio == BUTTON_PIN && events == GPIO_IRQ_EDGE_RISE){
      //Button_Flag = 1;
      prevEdgeTime = get_absolute_time();
      buttonEdge = RISING_EDGE;
      //buttonAlarm = add_alarm_in_ms(40, &debounceTimer, NULL ,true); 
    }
    else if(gpio == BUTTON_PIN && events == GPIO_IRQ_EDGE_FALL){
        prevEdgeTime = get_absolute_time();
        buttonEdge = FALLING_EDGE;
    }
}

int main() {
    //Button Setup
    gpio_set_irq_enabled_with_callback(BUTTON_PIN,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &buttonISR);
    volatile uint32_t currentTime = to_ms_since_boot(get_absolute_time());
    uint Button_Flag = 0;

    systemStates currentState = vehicleOff;
    MFRC522 mfrc522(RFID_CS, RFID_RST);
    //authUsers[1].size = 4;
    users authUsers;
    authUsers.numUsers = 2;
    authUsers.userList[0] = {4, {0x33, 0x0c, 0x24, 0x94}};
    authUsers.userList[1] = {4, {0xDE, 0xAD, 0xC0, 0xDE}};

   //Base colors, low intensity 
    const uint32_t red = urgb_u32(0x05,0,0,0);
    const uint32_t green = urgb_u32(0,0x05,0,0);
    const uint32_t blue = urgb_u32(0,0,0x05,0);

    repeating_timer_t timer;
    stdio_init_all();
    ConsoleInit();
    add_repeating_timer_ms(20, timerCallback, NULL, &timer);

    int16_t accels[3];

    systemInit(timer, mfrc522);

    writeDatabase(authUsers);
    busy_wait_ms(5);
    users TestRead;
    TestRead = readDatabase();

    //Need to implement a method that checks if the card is still present to prevent repeated reads
    while(1) {
        switch(currentState){
            case vehicleOff:
                if(mfrc522.PICC_IsNewCardPresent()){
                    if(mfrc522.PICC_ReadCardSerial()){
                        currentState = cardPresented;
                    }
                }

                if(Button_Flag == 1){
                    currentState = consoleMode;
                    Button_Flag = 0;
                }
                break;
            case cardPresented:
                if(compareUIDs(authUsers, mfrc522)){
                    currentState = Authorized;
                }
                else{currentState = notAuthorized;}
                break;
            case Authorized:
                //Flash green LED
                //Enable Relay
                closeRelay(CLOSE_PIN);
                break;
            case notAuthorized:
                //Flash red LED
                //Disable Relay
                openRelay(OPEN_PIN);
                break;
            case vehicleOn:
                //If button is pressed, shutdown
                //If authorized card is presented, shutdown
                //Have some hysterisis in startup/shutdown cycles
                
                break;
            case vehicleShutdown:

                break;
            case consoleMode:
                if( timerFlag == 1 ){
                    ConsoleProcess();
                    timerFlag = 0;
                }
                //Exit if button is pressed
                if(Button_Flag == 1){
                    currentState = vehicleOff;
                    Button_Flag = 0;
                }
                break;
        }

        
        if (mfrc522.PICC_IsNewCardPresent()) {
            if(mfrc522.PICC_ReadCardSerial()){
                if(compareUIDs(authUsers, mfrc522))
                {
                    puts("UID MATCHES");
                }
                else{puts("NOT AUTHORIZED");}
                puts("UID:");
                for (int i = 0; i<mfrc522.uid.size; i++){
                    printf("%X", mfrc522.uid.uiduint8_t[i]);
                }
                puts("\n");
            }   
        }

        gpio_put(LED_PIN, 0);
        put_pixel(red);
        accl_read(accels);

/*         printf("Accel: X = %d.%d, Y = %d.%d, Z = %d.%d\n", 
            accels[0]/1000,accels[0]%1000, 
            accels[1]/1000,accels[1]%1000,
            accels[2]/1000,accels[2]%1000); */

        //printf("Accel: X = %d, Y = %d, Z = %d\n", accels[0], accels[1], accels[2]);

        //closeRelay(CLOSE_PIN);
        busy_wait_ms(20);
        put_pixel(blue);
        //openRelay(OPEN_PIN);
        //gpio_put(LED_PIN, 1);
        //puts("Hello World\n");
        busy_wait_ms(20);

        //Check if button has been presed
        currentTime = to_ms_since_boot(get_absolute_time());
        if(buttonEdge == RISING_EDGE && (currentTime-to_ms_since_boot(prevEdgeTime)) > BUTTON_DEB_TIME){
            Button_Flag = 1;
            buttonEdge = 0;
        }
    }
    return 0;
}

void systemInit(repeating_timer consoleTimer, MFRC522 rfid){
    //Console
    
    
    //Onboard RP2040 LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    //Neopixel
    neopixel_init(PIN_TX);
    //Relay
    relayInit(CLOSE_PIN, OPEN_PIN);
    openRelay(OPEN_PIN);
    //Accelerometer
    accl_I2C_Init();
    accl_wakeup();
    //RFID Reader
    rfid.PCD_Init();
    
}

void BUTTON_INIT(){
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);
}

bool compareUIDs(users userDataBase, MFRC522 rfidData){

    for(int n = 0; n<userDataBase.numUsers; n++){ //Check against each authorized user
        for(int i = 0; i<3; i++){ //Compare read UID to the authorized user
            if(rfidData.uid.uiduint8_t[i] == userDataBase.userList[n].uiduint8_t[i]){
                return true; //If they match, user is authorized
            }
        }
    }
    return false; //Otherwise the user is not authorized
}

void writeDatabase(users userDatabase){
    uint arrElements = (userDatabase.numUsers*4) + 1;
    uint8_t writeData[arrElements];
    writeData[0] = userDatabase.numUsers; //The first element is always the number of valid users
    for(int i = 0; i<userDatabase.numUsers; i++){
        for(int n = 0; n<4; n++){ //Each user UID is 4 bytes.
            //Write Data index explanation:Offset by 1 due to first element being the number of users
            //i*4: each user is 4 bytes
            //+n: data is sent byte by byte to the EEPROM
            writeData[i*4+1+n] = userDatabase.userList[i].uiduint8_t[n];
        }
    }
    if(arrElements > 32){ //A single page write can be no larger than 32 bytes
        uint writeNum = (arrElements/32) + 1;
        uint8_t txBuffer[32];
        uint16_t startAddress;
        for(int i = 0; i<arrElements; i++){
            startAddress = i*32;
            for(int n = 0; n<32; n++){
                txBuffer[n] = writeData[(i*32)+n];
            }
            eepromPageWrite(txBuffer, startAddress, sizeof(txBuffer));
        }
    }
    else{
        eepromPageWrite(writeData, 0, sizeof(writeData));
    }
    busy_wait_ms(1);
}

users readDatabase(){
    users eepromData;
    eepromData.numUsers = eepromByteRead(0); //user count is stored in the first byte


    uint8_t eepromPageData[totalNumUsers*4]; //4 bytes per user
    eepromPageRead(eepromPageData,1,4*eepromData.numUsers);

    for(int i = 0; i<eepromData.numUsers; i++){
        eepromData.userList[i].size = 4;
        for(int n = 0; n<4; n++){
            eepromData.userList[i].uiduint8_t[n] = eepromPageData[((i*4)+n)];
        }
    }
    return eepromData;
}
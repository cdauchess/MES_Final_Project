/* 
    This code originates from the Getting started with Raspberry Pi Pico document
    https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
    CC BY-ND Raspberry Pi (Trading) Ltd
*/
#include "main.h"

uint LED_PIN = 25; //Onboard RP2040 LED
uint PIN_TX = 15; //Neopixel Pin
const uint CLOSE_PIN = 19; //Relay open pin
const uint OPEN_PIN = 18; //Relay close pin



//Card: 33C2494



uint32_t timerFlag = 0;

bool timerCallback(repeating_timer_t *rt){
    timerFlag = 1;
    return 1;
}

int main() {
    systemStates currentState = vehicleOff;
    MFRC522 mfrc522(RFID_CS, RFID_RST);
    //authUsers[1].size = 4;
    users authUsers;
    authUsers.numUsers = 1;
    authUsers.userList[0] = {4, {0x33, 0x0c, 0x24, 0x94}};

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

    //Need to implement a method that checks if the card is still present to prevent repeated reads
    while(1) {
        switch(currentState){
            case vehicleOff:
                if(mfrc522.PICC_IsNewCardPresent()){
                    if(mfrc522.PICC_ReadCardSerial()){
                        currentState = cardPresented;
                    }
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
/* 
    This code originates from the Getting started with Raspberry Pi Pico document
    https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
    CC BY-ND Raspberry Pi (Trading) Ltd
*/
#include "main.h"

//LED GUIDE:
//RED: Vehicle Off
//GREEN: Vehcle On/Authorized
//BLUE: Console Mode
//WHITE: Not authorized

//Hardware Definitions
const uint LED_PIN = 25; //Onboard RP2040 LED
const uint PIN_TX = 15; //Neopixel Pin
const uint BUTTON_PIN = 14; //GPIO For button input
const uint CLOSE_PIN = 18; //Relay open pin
const uint OPEN_PIN = 19; //Relay close pin

//Button ISR Flag(s)
uint buttonEdge = 0; //1 for rising, 2 for falling
absolute_time_t prevEdgeTime;

//Card: 33C2494

uint8_t exitConsole = 0;


uint8_t timerFlag = 0;
uint8_t pixelFlag = 0;
uint8_t accelFlag = 0;
uint onOffTime = 0;

bool timerCallback(repeating_timer_t *rt){
    timerFlag = 1;
    return 1;
}

bool pixelCallback(repeating_timer_t *rt){
    pixelFlag = 1;
    onOffTime++;
    return 1;
}

bool accelTimCallback(repeating_timer_t *rt){
    accelFlag = 1;
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
    //Neopixel Setup
    repeating_timer_t pixelTimer;
    add_repeating_timer_ms(100,pixelCallback, NULL, &pixelTimer);
    uint8_t flashCount = 0;
    uint8_t pixelStatus = 0;
    //Accel Timer setup
    repeating_timer_t accelTimer;
    add_repeating_timer_ms(50,accelTimCallback, NULL, &accelTimer);
    accels accelRes[ACCEL_SAMPLES];

    systemStates currentState = vehicleOff;
    MFRC522 mfrc522(RFID_CS, RFID_RST);
    //authUsers[1].size = 4;
    users authUsers;
    authUsers.numUsers = 2;
    authUsers.userList[0] = {4, {0x33, 0x0c, 0x24, 0x94}};
    authUsers.userList[1] = {4, {0xDE, 0xAD, 0xC0, 0xDE}};

   //Base colors, low intensity 
    const uint32_t red = urgb_u32(1*BRIGHTNESS,0,0,0);
    const uint32_t green = urgb_u32(0,1*BRIGHTNESS,0,0);
    const uint32_t blue = urgb_u32(0,0,1*BRIGHTNESS,0);
    const uint32_t white = urgb_u32(0,0,0,1*BRIGHTNESS);

    repeating_timer_t timer;
    stdio_init_all();
    //ConsoleInit();
    add_repeating_timer_ms(20, timerCallback, NULL, &timer);

    

    systemInit(timer, mfrc522);

    writeDatabase(authUsers);
    busy_wait_ms(5);
    users TestRead;
    authUsers = readDatabase();
    put_pixel(red);
    openRelay(OPEN_PIN);

    while(1) {
        switch(currentState){
            case vehicleOff:
                //put_pixel(red);
                
                if(mfrc522.PICC_IsNewCardPresent()){
                    if(mfrc522.PICC_ReadCardSerial()){
                        currentState = cardPresented;
                    }
                }

                if(Button_Flag == 1){
                    currentState = consoleMode;
                    Button_Flag = 0;
                    put_pixel(blue);
                    ConsoleInit();
                }
                break;
            case cardPresented:
                if(compareUIDs(authUsers, mfrc522)){
                    currentState = Authorized;
                }
                else{currentState = notAuthorized;}
                break;
            case Authorized:
                if(pixelFlag && !pixelStatus){
                    put_pixel(green);
                    pixelFlag = 0;
                    pixelStatus = 1;
                    flashCount++;
                }
                if(pixelFlag && pixelStatus){
                    put_pixel(0);
                    pixelFlag = 0;
                    pixelStatus = 0;                    
                }
                if(flashCount > FlASHES){
                    //put_pixel(green);
                    currentState = vehicleOn;
                    flashCount = 0;
                    pixelStatus = 1;
                    onOffTime = 0;
                    closeRelay(CLOSE_PIN);
                }

                //Flash green LED
                //Enable Relay
                
                break;
            case notAuthorized:
                //Flash red LED
                //Disable Relay
                put_pixel(white);
                currentState = vehicleOff;
                break;
            case vehicleOn:
                //If button is pressed, shutdown
                //If authorized card is presented, shutdown
                //Have some hysterisis in startup/shutdown cycles
                if(Button_Flag == 1){
                    currentState = vehicleShutdown;
                    Button_Flag = 0;
                }    
                if(checkInact(accelRes) && (onOffTime > MIN_ON_TIME)){
                    currentState = vehicleShutdown;
                }            
                break;
            case vehicleShutdown:
                openRelay(OPEN_PIN);
                put_pixel(red);
                currentState = vehicleOff;
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
                    put_pixel(red);
                }
                if(exitConsole == 1){
                    currentState = vehicleOff;
                    put_pixel(red);
                    exitConsole = 0;
                }
                break;
        } //End currentState switch statement

        if(accelFlag == 1){
            for(int i = ACCEL_SAMPLES-1; i > 0; i--){
                accelRes[i] = accelRes[i-1]; //Move everything one element out in the array, overwrite the final element
            }
            accelRes[0] = accl_read();
            accelFlag = 0;
        }
        //Check if button has been presed
        currentTime = to_ms_since_boot(get_absolute_time());
        if(buttonEdge == RISING_EDGE && (currentTime-to_ms_since_boot(prevEdgeTime)) > BUTTON_DEB_TIME){
            Button_Flag = 1;
            buttonEdge = 0;
        }
    } //End while(1)
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



bool checkInact(accels accelVals[ACCEL_SAMPLES]){
    int compareSamp = 0;
    int Xavg = 0;
    int Yavg = 0;
    int Zavg = 0;
    for(int i = 0; i<ACCEL_SAMPLES; i++){
        Xavg += accelVals[i].X;
        Yavg += accelVals[i].Y;
        Zavg += accelVals[i].Z;
    }

    Xavg = Xavg/ACCEL_SAMPLES;
    Yavg = Yavg/ACCEL_SAMPLES;
    Zavg = Zavg/ACCEL_SAMPLES;

    if(
        (abs(accelVals[compareSamp].X - Xavg) < AVG_TOL) &&
        (abs(accelVals[compareSamp].Y - Yavg) < AVG_TOL) &&
        (abs(accelVals[compareSamp].Z - Zavg) < AVG_TOL)
    ){ return true;} //Vehicle is inactive

    else{ return false;} //Vehicle is still moving
}
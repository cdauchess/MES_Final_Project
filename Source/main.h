#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "generated/ws2812.pio.h" //From RP2040 SDK examples

#include "MFRC522.h" //Adapted from arduino...should add a link to github repo here



#define RISING_EDGE 1
#define FALLING_EDGE 2
#define BUTTON_DEB_TIME 5 //Button debounce time in ms

//LED Settings
#define FlASHES 3
#define BRIGHTNESS 0x02

#define ACCEL_SAMPLES 100
#define AVG_TOL       50

#define MIN_ON_TIME 50

extern "C" {
    #include "Adxl343.h"
    #include "relay.h"
    #include "Neopixel.h" //From RP2040 SDK examples
    #include "console.h" //From Elecia White's demo...add github link here
    #include "CAT24C32.h"
    #include "userFuncs.h"
}




typedef enum {
    vehicleOff,
    cardPresented,
    Authorized, //Vehicle startup
    notAuthorized,
    vehicleOn,
    vehicleShutdown,
    consoleMode
}systemStates;

void systemInit(repeating_timer consoleTimer, MFRC522 rfid);
void BUTTON_INIT();

bool compareUIDs(users userDataBase, MFRC522 rfidData);

//See EEPROM Memory map for info on how data is stored


bool checkInact(accels accelVals[ACCEL_SAMPLES]);

#endif //MAIN_H
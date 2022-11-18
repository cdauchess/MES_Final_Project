#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "generated/ws2812.pio.h" //From RP2040 SDK examples

#include "MFRC522.h" //Adapted from arduino...should add a link to github repo here

#define totalNumUsers 256

#define RISING_EDGE 1
#define FALLING_EDGE 2
#define BUTTON_DEB_TIME 5 //Button debounce time in ms

//LED Settings
#define FlASHES 3
#define BRIGHTNESS 0x02

#define ACCEL_SAMPLES 100
#define AVG_TOL       10

#define MIN_ON_TIME 50

extern "C" {
    #include "Adxl343.h"
    #include "relay.h"
    #include "Neopixel.h" //From RP2040 SDK examples
    #include "console.h" //From Elecia White's demo...add github link here
    #include "CAT24C32.h"
}


typedef struct {
		uint8_t		size;			// Number of uint8_ts in the UID. 4, 7 or 10.
		uint8_t		uiduint8_t[4];
		//uint8_t		sak;			// The SAK (Select acknowledge) uint8_t returned from the PICC after successful selection.
} Uid;

typedef struct {
    uint8_t numUsers;
    Uid userList[totalNumUsers]; //Provisioning for 100 users currently.  
}users;

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
void writeDatabase(users userDataBase);
users readDatabase();

bool checkInact(accels accelVals[ACCEL_SAMPLES]);
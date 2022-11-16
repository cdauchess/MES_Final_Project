#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "generated/ws2812.pio.h" //From RP2040 SDK examples

#include "MFRC522.h" //Adapted from arduino...should add a link to github repo here

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
    uint16_t numUsers;
    Uid userList[100]; //Provisioning for 100 users currently.  
}users;

void systemInit(repeating_timer consoleTimer, MFRC522 rfid);

bool compareUIDs(users userDataBase, MFRC522 rfidData);
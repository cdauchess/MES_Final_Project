#ifndef USERFUNCS_H
#define USERFUNCS_H

#include "CAT24C32.h"

#define totalNumUsers 256

typedef struct {
		uint8_t		size;			// Number of uint8_ts in the UID. 4, 7 or 10.
		uint8_t		uiduint8_t[4];
		//uint8_t		sak;			// The SAK (Select acknowledge) uint8_t returned from the PICC after successful selection.
} Uid;

typedef struct {
    uint8_t numUsers;
    Uid userList[totalNumUsers]; //Provisioning for 100 users currently.  
}users;

void writeDatabase(users userDataBase);
users readDatabase();

#endif //USERFUNCS_H
#include "userFuncs.h"

//Write the user data base to the EEPROM
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

//Read the database from the EEPROM
//Returns the read user database
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
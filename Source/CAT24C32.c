#include "CAT24C32.h"

//Byte Write
int eepromByteWrite(uint8_t wByte, uint16_t wAddr){
    //The address is a 2 byte value
    uint8_t dataToSend[3] = {((wAddr >> 8) & 0xff), (wAddr & 0xff), wByte};
    //send three bytes, each address byte followed by the data to write
    i2c_write_blocking(EEPROM_I2C,EEPROM_ADDR, dataToSend, sizeof(dataToSend), false);
    return 1;
}

//Page Write
int eepromPageWrite(uint8_t wBytes [], uint16_t wAddr, uint8_t wLength){
    //Send two address bytes followed by the sequential data bytes in a single transaction.
    uint16_t totalLength = 2 + wLength;
    uint8_t dataToSend[totalLength];
    //Fill the first two bytes with the starting address of data to write
    dataToSend[0] = (wAddr >> 8) & 0xff;
    dataToSend[1] = (wAddr & 0xff); 
    //Fill the remainder with the actual data to write sequentially
    for(int i = 2; i < totalLength; i++){
        dataToSend[i] = wBytes[i-2];
    }

    i2c_write_blocking(EEPROM_I2C,EEPROM_ADDR, dataToSend, totalLength, false);
    return 1;
}

//Byte Read
uint8_t eepromByteRead(uint16_t rAddr){
    uint8_t readData = 0;
    uint8_t addrToSend[2] = {((rAddr >> 8) & 0xff), (rAddr & 0xff)};
    //Send the two address bytes, followed by a restart and a read
    i2c_write_blocking(EEPROM_I2C, EEPROM_ADDR, addrToSend, sizeof(addrToSend),true );
    i2c_read_blocking(EEPROM_I2C,EEPROM_ADDR,&readData, 1, false);
    return readData;
}

//Page Read
//Be careful here, rData must be large enough to contain all of the data.
int eepromPageRead(uint8_t *rData, uint16_t rAddr, uint16_t rLength){
    uint8_t addrToSend[2] = {((rAddr >> 8) & 0xff), (rAddr & 0xff)};
    i2c_write_blocking(EEPROM_I2C, EEPROM_ADDR, addrToSend, sizeof(addrToSend),true );

    i2c_read_blocking(EEPROM_I2C,EEPROM_ADDR,rData, rLength, false);
    return 1;
}
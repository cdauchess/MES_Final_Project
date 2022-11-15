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
int eepromPageWrite(uint8_t wBytes [], uint16_t wAddr){

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
int eepromPageRead(uint8_t *rData, uint16_t rAddr, uint16_t rLength){

}
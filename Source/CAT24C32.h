#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define EEPROM_ADDR 0x50
#define EEPROM_I2C  i2c1



//Byte Write
int eepromByteWrite(uint8_t wByte, uint16_t wAddr);

//Page Write
int eepromPageWrite(uint8_t wBytes [], uint16_t wAddr, uint8_t wLength);

//Byte Read
uint8_t eepromByteRead(uint16_t rAddr);

//Page Read
int eepromPageRead(uint8_t *rData, uint16_t rAddr, uint16_t rLength);
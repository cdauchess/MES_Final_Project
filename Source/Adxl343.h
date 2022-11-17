#ifndef ADXL343_H
#define ADXL343_H 

#include "pico/stdlib.h"
#include "hardware/i2c.h"



#define ACCL_ADDR 0x53

//ADXL 343 Registers
#define ADXL_BW_RATE        0x2C
#define ADXL343_DATA_FORMAT 0x31
#define ADXL343_FIFO_CTL    0x38
#define ADXL343_FIFO_STATUS 0x39

//SDA: GPIO 2
//SCL: GPIO 3
//INT2: GPIO 4
//INT1: GPIO5

//Function to initialize I2C
int accl_I2C_Init();

int accl_write_reg(uint8_t regAddr, uint8_t regData);

int accl_wakeup();

int accl_read(int16_t accel[3]);

#endif //ADXL343_H
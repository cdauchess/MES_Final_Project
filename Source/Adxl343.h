#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define ACCL_ADDR 0x53


//SDA: GPIO 2
//SCL: GPIO 3
//INT2: GPIO 4
//INT1: GPIO5

//Function to initialize I2C
int RFID_I2C_Init();

int accl_wakeup();

int accl_read(int16_t accel[3]);
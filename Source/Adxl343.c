#include "Adxl343.h"
//SDA: GPIO 2
//SCL: GPIO 3
//INT2: GPIO 4
//INT1: GPIO5


const int sdaPin = 2;
const int sclPin = 3;
const int acclBaud = 100000;



int RFID_I2C_Init(){
    i2c_init(i2c1,acclBaud);
    gpio_set_function(sdaPin, GPIO_FUNC_I2C);
    gpio_set_function(sclPin, GPIO_FUNC_I2C);
    gpio_pull_up(sdaPin);
    gpio_pull_up(sclPin);
}

//Accl wake up
int accl_wakeup(){
    uint8_t measureBit = 3;
    uint8_t powerReg = 0x2D;
    uint8_t regData = 0x8;
    uint8_t buffer = 0;
    uint8_t txBuffer[] = {powerReg, regData};
    //regData &= 0x1<<measureBit;
    i2c_write_blocking(i2c1,ACCL_ADDR,0x00,1,true);
    i2c_read_blocking(i2c1,ACCL_ADDR,&buffer,1,false);

    i2c_write_blocking(i2c1,ACCL_ADDR,txBuffer,2,false);
    //i2c_write_blocking(i2c1,ACCL_ADDR,&regData,1,false);

    i2c_write_blocking(i2c1,ACCL_ADDR,&powerReg,1,true);
    i2c_read_blocking(i2c1,ACCL_ADDR,&buffer,1,false);
}
//Stream mode?  Time isn't super sensitive

//0x32 - 0x37 are the data registers
int accl_read(int16_t accel[3]){
    uint8_t dataStart = 0x32;
    uint8_t buffer[6];
    i2c_write_blocking(i2c1,ACCL_ADDR,&dataStart,1,true);
    i2c_read_blocking(i2c1, ACCL_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2 + 1] << 8 | buffer[(i * 2)])/4;
    }

    return 0;
}
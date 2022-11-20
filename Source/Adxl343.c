#include "Adxl343.h"
//SDA: GPIO 2
//SCL: GPIO 3
//INT2: GPIO 4
//INT1: GPIO5


const int sdaPin = 2;
const int sclPin = 3;
const int acclBaud = 100000;



int accl_I2C_Init(){
    i2c_init(i2c1,acclBaud);
    gpio_set_function(sdaPin, GPIO_FUNC_I2C);
    gpio_set_function(sclPin, GPIO_FUNC_I2C);
    gpio_pull_up(sdaPin);
    gpio_pull_up(sclPin);
}

int accl_write_reg(uint8_t regAddr, uint8_t regData){
    uint8_t txBuffer[] = {regAddr, regData};
    i2c_write_blocking(i2c1,ACCL_ADDR,txBuffer,2,false);
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

    uint8_t dataFormat = 0b00001000; //Operate in full resolution mode
    accl_write_reg(ADXL343_DATA_FORMAT, dataFormat);

    uint8_t fifoWrite = 0b00000000; //bypass mode
    accl_write_reg(ADXL343_FIFO_CTL,fifoWrite);

    uint8_t bwCtl = 0b00001000; //25Hz
    accl_write_reg(ADXL_BW_RATE, bwCtl);

    i2c_write_blocking(i2c1,ACCL_ADDR,txBuffer,2,false);
    //i2c_write_blocking(i2c1,ACCL_ADDR,&regData,1,false);

    i2c_write_blocking(i2c1,ACCL_ADDR,&powerReg,1,true);
    i2c_read_blocking(i2c1,ACCL_ADDR,&buffer,1,false);
}

//0x32 - 0x37 are the data registers
//0:X, 1:Y, 2:Z
accels accl_read(){
    accels tempAccel;
    uint8_t dataStart = 0x32;
    uint8_t buffer[6];
    float scaleFactor = 3.9;
    i2c_write_blocking(i2c1,ACCL_ADDR,&dataStart,1,true);
    i2c_read_blocking(i2c1, ACCL_ADDR, buffer, 6, false);

    //Convert to milli G.  This should allow for decent display resolution while not working with floats.
    //Comes from ADXL343 as 3.9 mg/bit
    int tempVal;
    tempAccel.X = buffer[1] << 8 | buffer[0];
    tempAccel.X *= scaleFactor;
    tempAccel.Y = buffer[3] << 8 | buffer[2];
    tempAccel.Y *= scaleFactor;
    tempAccel.Z = buffer[5] << 8 | buffer[4];
    tempAccel.Z *=scaleFactor;
    return tempAccel;
}
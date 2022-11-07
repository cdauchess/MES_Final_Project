#include "MFRC522_RP2040.h"

/*
Interrupt Pin: GPIO8
RESET Pin: GPIO9
SPI1
SCK: GPIO10
MOSI: GPIO11
MISO: GPIO12
RFID_CS: GPIO13
*/


uint8_t MFRC522_ReadReg(uint8_t RegAddr){

    uint8_t txRegAddr = (1 << 7) | RegAddr; //Set MSB to 1 for a read
    uint8_t readData = 0;

    spi_write_read_blocking(spi1, &txRegAddr, &readData, 1);
    txRegAddr = 0x00;
    spi_write_read_blocking(spi1, &txRegAddr, &readData, 1);
    //printf("Data Return: %00x \n", readData);
    return readData;
}

void MFRC522_WriteReg(uint8_t RegAddr, uint8_t Data){
    uint8_t txRegAddr = RegAddr; //MSB is a 0 for a write
    spi_write_blocking(spi1, &txRegAddr, 1);
    spi_write_blocking(spi1, &Data, 1);
}

//TODO Multi byte writes and reads

void MFRC522_HW_INIT(){
    spi_init(spi1,500*1000);
    //Set pin modes for SPI
    gpio_set_function(RFID_SCK, GPIO_FUNC_SPI);
    gpio_set_function(RFID_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(RFID_MISO, GPIO_FUNC_SPI);
    gpio_set_function(RFID_CS, GPIO_FUNC_SPI);

    //Reset the chip
    gpio_set_dir(RFID_RST,true);
    gpio_put(RFID_RST, 0);
    busy_wait_us(2); //Requires 100ns of low
    gpio_put(RFID_RST, 1);
    busy_wait_us(70);//Requires 37.74us for oscillator startup
}

/* void MFRC522_INIT(){
    uint8_t Version = 0;
    MFRC522_HW_INIT();
    Version = MFRC522_ReadReg(VersionReg);
    printf("MFRC522 Version: 0x%00x \n", Version);


}
 */
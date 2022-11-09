
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#define RFID_INT     8
#define RFID_RST     9
#define RFID_SCK    10
#define RFID_MOSI   11
#define RFID_MISO   12
#define RFID_CS     13
#define SS RFID_CS          //Catch all for when standard SS is used

#define LOW 0
#define HIGH 1

#define RFID_SPI spi1

#define OUTPUT true
#define INPUT false

void digitalWrite(uint PIN, bool STATE);
bool digitalRead(uint PIN);

uint32_t millis();
void yield();
void delayMicroseconds(uint32_t delayTime);
void delay(uint32_t delayTime);
void pinMode(uint PIN, bool MODE);

class spi {
    public:
        void spiInit(uint baudrate, uint SCK_PIN, uint MOSI_PIN, uint MISO_PIN);
        uint8_t transfer(uint8_t data);  
};

class SERIAL {
    public:
        void print();
};
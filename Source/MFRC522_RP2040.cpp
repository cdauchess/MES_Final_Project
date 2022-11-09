
#include "MFRC522_RP2040.h"

void digitalWrite(uint PIN, bool STATE){
    gpio_put(PIN, STATE);
}

bool digitalRead(uint PIN){
    return gpio_get(PIN);
}

uint32_t millis(){
    return to_ms_since_boot(get_absolute_time());
}

void yield(){
    return;
}

void delayMicroseconds(uint32_t delayTime){
    busy_wait_us(delayTime);
}

void delay(uint32_t delayTime){
    busy_wait_ms(delayTime);
}

void pinMode(uint PIN, bool MODE){
    gpio_init(PIN);
    gpio_set_dir(PIN, MODE);
}

uint8_t spi::transfer(uint8_t data){
    uint8_t read;
    spi_write_read_blocking(RFID_SPI, &data, &read, 1);
    return read;
}

void spi::spiInit(uint baudrate, uint SCK_PIN, uint MOSI_PIN, uint MISO_PIN){
    spi_init(RFID_SPI, baudrate);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
}
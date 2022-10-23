/* 
    This code originates from the Getting started with Raspberry Pi Pico document
    https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
    CC BY-ND Raspberry Pi (Trading) Ltd
*/
#include "main.h"

uint LED_PIN;
uint PIN_TX = 15;
const uint CLOSE_PIN = 19;
const uint OPEN_PIN = 18;

volatile uint initGlobal = 5;
volatile uint nonInitGlobal;

int main() {
    bi_decl(bi_program_description("PROJECT DESCRIPTION"));
    
    stdio_init_all();

    uint32_t red = urgb_u32(0x0f,0,0,0);
    uint32_t green = urgb_u32(0,0x0f,0,0);
    uint32_t blue = urgb_u32(0,0,0x0f,0);
    LED_PIN = 25;

    int16_t accels[3];

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    neopixel_init(PIN_TX);

    relayInit(CLOSE_PIN, OPEN_PIN);

    RFID_I2C_Init();
    accl_wakeup();



    while(1) {
        gpio_put(LED_PIN, 0);
        put_pixel(red);
        accl_read(&accels);
         printf("Acc. X = %d, Y = %d, Z = %d\n", accels[0], accels[1], accels[2]);
        //closeRelay(CLOSE_PIN);
        busy_wait_ms(1000);
        put_pixel(blue);
        //openRelay(OPEN_PIN);
        gpio_put(LED_PIN, 1);
        puts("Hello World\n");
        busy_wait_ms(1000);
    }
}
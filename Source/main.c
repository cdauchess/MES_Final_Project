/* 
    This code originates from the Getting started with Raspberry Pi Pico document
    https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
    CC BY-ND Raspberry Pi (Trading) Ltd
*/
#include "main.h"

const uint LED_PIN = 25;
const uint PIN_TX = 15;

int main() {
    bi_decl(bi_program_description("PROJECT DESCRIPTION"));
    
    stdio_init_all();

    uint32_t red = urgb_u32(0x0f,0,0,0);
    uint32_t green = urgb_u32(0,0x0f,0,0);
    uint32_t blue = urgb_u32(0,0,0x0f,0);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    neopixel_init(PIN_TX);



    while(1) {
        gpio_put(LED_PIN, 0);
        put_pixel(red);
        sleep_ms(250);
        put_pixel(blue);
        gpio_put(LED_PIN, 1);
        puts("Hello World\n");
        sleep_ms(1000);
    }
}
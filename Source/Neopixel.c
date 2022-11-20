#include "Neopixel.h"

//Initialize the PIO system
void neopixel_init(uint PIN_TX){ 
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, PIN_TX, 800000, true);
}

//Send pixel data out
void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb);
}

//Formate the 32 bit uint that contains the RGBW color and intensity information
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return
            ((uint32_t) (r) << 16) |
            ((uint32_t) (g) << 24) |
            ((uint32_t) (b) << 8)  |
            ((uint32_t) (w) << 0);
}
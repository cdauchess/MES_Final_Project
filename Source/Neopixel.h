
#include "generated/ws2812.pio.h"
#include "hardware/pio.h"

void neopixel_init(uint PIN_TX);
void put_pixel(uint32_t pixel_grb);
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
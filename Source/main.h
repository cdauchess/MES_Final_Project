#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "generated/ws2812.pio.h"

#include "MFRC522_RP2040.h"
#include "MFRC522.h"

extern "C" {
    #include "Adxl343.h"
    #include "relay.h"
    #include "Neopixel.h"

}
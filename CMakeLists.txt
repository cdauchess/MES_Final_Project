cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(main)

pico_sdk_init()

add_executable(main    
    source/main.cpp
    source/Neopixel.c
    source/relay.c
    source/Adxl343.c
    source/MFRC522_RP2040.cpp
    source/MFRC522.cpp
)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(main ${CMAKE_CURRENT_LIST_DIR}/source/PIO/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/source/generated)

pico_enable_stdio_usb(main 0)
pico_enable_stdio_uart(main 1)

pico_add_extra_outputs(main)

target_link_libraries(main PRIVATE pico_stdlib hardware_pio hardware_i2c hardware_spi)
pico_add_extra_outputs(main)

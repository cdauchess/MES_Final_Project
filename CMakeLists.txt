cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pio_ws2812)

pico_sdk_init()

# add_executable(pio_ws2812    
#     pio_ws2812.c
# )

# pico_generate_pio_header(pio_ws2812 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# pico_enable_stdio_usb(main 0)
# pico_enable_stdio_uart(main 1)

# pico_add_extra_outputs(main)

# #target_link_libraries(main pico_stdlib)
# target_link_libraries(pio_ws2812 PRIVATE pico_stdlib hardware_pio)
# pico_add_extra_outputs(pio_ws2812)

add_executable(pio_ws2812)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(pio_ws2812 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_sources(pio_ws2812 PRIVATE ws2812.c)

target_link_libraries(pio_ws2812 PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(pio_ws2812)

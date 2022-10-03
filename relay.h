//Relay output handlers.
//This serves to seperate the application from the hardware.  

#ifndef RELAY_H
#define RELAY_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define COIL_DELAY 20 //delay in ms to hold the pin high to set the coil

alarm_callback_t closeTimerCallback(alarm_id_t id, void*closePin);
alarm_callback_t openTimerCallback(alarm_id_t id, void*openPin);
void relayInit(int closePin, int openPin);
void closeRelay(int closePin);
void openRelay(int openPin);

#endif //RELAY_H
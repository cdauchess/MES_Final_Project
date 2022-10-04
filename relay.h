//Relay output handlers.
//This serves to seperate the application from the hardware.  

#ifndef RELAY_H
#define RELAY_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define COIL_DELAY 20 //delay in ms to hold the pin high to set the coil

alarm_callback_t closeTimerCallback(alarm_id_t id, void*closePin);
alarm_callback_t openTimerCallback(alarm_id_t id, void*openPin);

/*! \brief Initialize output relay
 *  \ingroup relay
 *
 * \param closePin RP2040 pin driving the relay close input
 * \param openPin RP2040 pin driving the relay open input
 */
void relayInit(int closePin, int openPin);
void closeRelay(int closePin);

/// @brief Open the relay
/// @param openPin RP2040 pin driving the relay open input
void openRelay(int openPin);

#endif //RELAY_H
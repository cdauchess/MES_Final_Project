
#include "relay.h"

alarm_callback_t closeTimerCallback(alarm_id_t id, void*closePin){
    gpio_put(closePin,0);
}

alarm_callback_t openTimerCallback(alarm_id_t id, void*openPin){
    gpio_put(openPin,0);
}

void relayInit(int closePin, int openPin){
    gpio_init(closePin);
    gpio_set_dir(closePin, GPIO_OUT);

    gpio_init(openPin);
    gpio_set_dir(openPin, GPIO_OUT);
}

void closeRelay(int closePin){
    gpio_put(closePin, 1);
    add_alarm_in_ms(COIL_DELAY, closeTimerCallback, closePin, true);
}

void openRelay(int openPin){
    gpio_put(openPin, 1);
    add_alarm_in_ms(COIL_DELAY, openTimerCallback, openPin, true);    
}

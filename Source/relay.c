
#include "relay.h"

alarm_callback_t closeTimerCallback(alarm_id_t id, void*user_data){
    gpio_put(18,0);
    return 0;
}

alarm_callback_t openTimerCallback(alarm_id_t id, void*user_data){
    gpio_put(19,0);
    return 0;
}

void relayInit(uint closePin, uint openPin){
    gpio_init(closePin);
    gpio_set_dir(closePin, GPIO_OUT);

    gpio_init(openPin);
    gpio_set_dir(openPin, GPIO_OUT);
}

void closeRelay(uint closePin){
    gpio_put(closePin, 1);
    add_alarm_in_ms(COIL_DELAY, closeTimerCallback, NULL, true);
    return;
}

void openRelay(uint openPin){
    gpio_put(openPin, 1);
    add_alarm_in_ms(COIL_DELAY, openTimerCallback, NULL, true);  
    return;  
}

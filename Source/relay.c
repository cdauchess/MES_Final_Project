
#include "relay.h"

//Alarm for the close action to release the output
alarm_callback_t closeTimerCallback(alarm_id_t id, void*user_data){
    gpio_put(18,0);
    return 0;
}

//Alarm for the open action to release the output
alarm_callback_t openTimerCallback(alarm_id_t id, void*user_data){
    gpio_put(19,0);
    return 0;
}

//Initializations for the relay GPIOs
void relayInit(uint closePin, uint openPin){
    gpio_init(closePin);
    gpio_set_dir(closePin, GPIO_OUT);

    gpio_init(openPin);
    gpio_set_dir(openPin, GPIO_OUT);
}

//Close the relay
void closeRelay(uint closePin){
    gpio_put(closePin, 1);
    add_alarm_in_ms(COIL_DELAY, closeTimerCallback, NULL, true);
    return;
}

//Open the relay
void openRelay(uint openPin){
    gpio_put(openPin, 1);
    add_alarm_in_ms(COIL_DELAY, openTimerCallback, NULL, true);  
    return;  
}

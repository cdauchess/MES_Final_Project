/* 
    This code originates from the Getting started with Raspberry Pi Pico document
    https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
    CC BY-ND Raspberry Pi (Trading) Ltd
*/
#include "main.h"

uint LED_PIN;
uint PIN_TX = 15;
const uint CLOSE_PIN = 19;
const uint OPEN_PIN = 18;

volatile uint initGlobal = 5;
volatile uint nonInitGlobal;

MFRC522 mfrc522(RFID_CS, RFID_RST);

int main() {
    bi_decl(bi_program_description("PROJECT DESCRIPTION"));
    
    stdio_init_all();

    uint32_t red = urgb_u32(0x05,0,0,0);
    uint32_t green = urgb_u32(0,0x05,0,0);
    uint32_t blue = urgb_u32(0,0,0x05,0);
    LED_PIN = 25;

    int16_t accels[3];

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    neopixel_init(PIN_TX);

    //relayInit(CLOSE_PIN, OPEN_PIN);

    accl_I2C_Init();
    accl_wakeup();

    mfrc522.PCD_Init();

    while(1) {
        if (mfrc522.PICC_IsNewCardPresent()) {
            if(mfrc522.PICC_ReadCardSerial()){
                puts("UID:");
                for (int i = 0; i<10; i++){
                    printf("%d", mfrc522.uid.uiduint8_t[i]);
                }
                puts("\n");
            }   
        }

        gpio_put(LED_PIN, 0);
        put_pixel(red);
        accl_read(accels);

/*         printf("Accel: X = %d.%d, Y = %d.%d, Z = %d.%d\n", 
            accels[0]/1000,accels[0]%1000, 
            accels[1]/1000,accels[1]%1000,
            accels[2]/1000,accels[2]%1000); */

        //printf("Accel: X = %d, Y = %d, Z = %d\n", accels[0], accels[1], accels[2]);

        //closeRelay(CLOSE_PIN);
        busy_wait_ms(20);
        put_pixel(blue);
        //openRelay(OPEN_PIN);
        //gpio_put(LED_PIN, 1);
        //puts("Hello World\n");
        busy_wait_ms(20);
    }
    return 0;
}
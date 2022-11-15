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

uint32_t timerFlag = 0;

bool timerCallback(repeating_timer_t *rt){
    timerFlag = 1;
    return 1;
}

int main() {
    bi_decl(bi_program_description("PROJECT DESCRIPTION"));
    repeating_timer_t timer;
    stdio_init_all();
    ConsoleInit();
    add_repeating_timer_ms(20, timerCallback, NULL, &timer);
    

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

    uint8_t testData [4] = {0xDE,0xAD,0xC0,0xDE};
    uint8_t testReadData[4] = {0x00,0x00,0x00,0x00};

    uint8_t readData = 0;
    eepromByteWrite(0xCD, 0x15);
    busy_wait_ms(10);
    readData = eepromByteRead(0x15);
    printf("EEPROM DATA: %X \n", readData);

    eepromPageWrite(testData, 0x50);
    busy_wait_ms(10);
    eepromPageRead( testReadData, 0x50,4);

    printf("EEPROM PAGE DATA: %X%X%X%X \n",testReadData[0],testReadData[1],testReadData[2],testReadData[3]);


    while(1) {
        if( timerFlag == 1 ){
            ConsoleProcess();
            timerFlag = 0;
        }        
        if (mfrc522.PICC_IsNewCardPresent()) {
            if(mfrc522.PICC_ReadCardSerial()){
                puts("UID:");
                for (int i = 0; i<mfrc522.uid.size; i++){
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
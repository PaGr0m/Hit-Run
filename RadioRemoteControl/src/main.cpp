/** Libraries 
 * 
 *  TODO: 
 *      - ссылки на библиотеки
 *      - переделать путь для Settings
 */
#include <Arduino.h>
#include <printf.h>
#include <RF24.h>
#include <C:\Users\PaGrom\Desktop\Hit&Run\Libraries\Settings.h>


/** Define Pins 
 * 
 *  param: NRF
 *  param: Buttons
 */
#define PIN_NRF_CE  7
#define PIN_NRF_CSN 8

#define PIN_BUTTON_START                A0
#define PIN_BUTTON_STOP                 A1
#define PIN_BUTTON_SCORE_GREEN_UP       A2
#define PIN_BUTTON_SCORE_GREEN_DOWN     A3
#define PIN_BUTTON_SCORE_RED_UP         A4
#define PIN_BUTTON_SCORE_RED_DPWN       A5
#define PIN_BUTTON_UPDATE_SCORE         A6
#define PIN_BUTTON_UPDATE_TIMER         A7

#define DELAY_TIME 50


/** Initialize Variables 
 */
const uint8_t STATUS_BUTTON_START               = 0;
const uint8_t STATUS_BUTTON_SCORE_GREEN_UP      = 3;
const uint8_t STATUS_BUTTON_SCORE_GREEN_DOWN    = 4;
const uint8_t STATUS_BUTTON_SCORE_RED_UP        = 5;
const uint8_t STATUS_BUTTON_SCORE_RED_DOWN      = 6;
const uint8_t STATUS_BUTTON_UPDATE_SCORE        = 7;
const uint8_t STATUS_BUTTON_UPDATE_TIMER        = 8;
const uint8_t STATUS_BUTTON_STOP                = 9;


/** Initialize Objects 
 */
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);


/** Настройки радиоканала 
 *  param: RADIO_CHANNEL --> Указываем канал приёма данных (от 0 до 127), 
 *                           5 - значит приём данных осуществляется на частоте 2,405 ГГц 
 *                           (на одном канале может быть только 1 приёмник и до 6 передатчиков)
 *  param: RADIO_DATARATE --> Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), 
 *                            RF24_1MBPS - 1Мбит/сек
 *  param: RADIO_PALEVEL --> Указываем мощность передатчика 
 *                           (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
 */
void radioSettings() 
{
     
    radio.begin();
    radio.setChannel(RADIO_CHANNEL);  
    radio.setDataRate(RADIO_DATARATE);  
    radio.setPALevel(RADIO_PALEVEL);    
    radio.openWritingPipe(RADIO_ADDRESS_REMOTE_CONTROL);
    radio.printDetails();
}


void setup() 
{
    // Serial settings
    Serial.begin(SERIAL_BAUDRATE);
    printf_begin();
    printf("\n\r ***** RADIO REMOTE CONTROL ***** \n\r");

    // Radio settings
    radioSettings();

    // Wait for console opening
    delay(3000);
}


// TODO: возможно стоит убрать delay
void loop() 
{
    if (digitalRead(PIN_BUTTON_START) == HIGH) 
    {
        radio.write(&STATUS_BUTTON_START, sizeof(STATUS_BUTTON_START));
        printf("Button {START} was clicked!\n");
        delay(DELAY_TIME);
    }
    else if (digitalRead(PIN_BUTTON_STOP) == HIGH) 
    {
        radio.write(&STATUS_BUTTON_STOP, sizeof(STATUS_BUTTON_STOP));
        printf("Button {STOP} was clicked!\n");
        delay(DELAY_TIME);
    }
    else if (digitalRead(PIN_BUTTON_SCORE_GREEN_UP) == HIGH) 
    {
        radio.write(&PIN_BUTTON_SCORE_GREEN_UP, sizeof(PIN_BUTTON_SCORE_GREEN_UP));
        printf("Button {GREEN UP} was clicked!\n");
        delay(DELAY_TIME);
    }
    else if (digitalRead(PIN_BUTTON_SCORE_GREEN_DOWN) == HIGH)
    {
        radio.write(&PIN_BUTTON_SCORE_GREEN_DOWN, sizeof(PIN_BUTTON_SCORE_GREEN_DOWN));
        printf("Button {GREEN DOWN} was clicked!\n");
        delay(DELAY_TIME);
    }
    else if (digitalRead(PIN_BUTTON_SCORE_RED_UP) == HIGH)
    {
        radio.write(&PIN_BUTTON_SCORE_RED_UP, sizeof(PIN_BUTTON_SCORE_RED_UP));
        printf("Button {RED UP} was clicked!\n");
        delay(DELAY_TIME);
    }
    else if (digitalRead(STATUS_BUTTON_SCORE_RED_DOWN) == HIGH)
    {
        radio.write(&STATUS_BUTTON_SCORE_RED_DOWN, sizeof(STATUS_BUTTON_SCORE_RED_DOWN));
        printf("Button {RED DOWN} was clicked!\n");
        delay(DELAY_TIME);
    }
    else if (digitalRead(PIN_BUTTON_UPDATE_SCORE) == HIGH)
    {
        radio.write(&PIN_BUTTON_UPDATE_SCORE, sizeof(PIN_BUTTON_UPDATE_SCORE));
        printf("Button {UPDATE SCORE} was clicked!\n");
        delay(DELAY_TIME);
    }
    else if (digitalRead(PIN_BUTTON_UPDATE_TIMER) == HIGH)
    {
        radio.write(&PIN_BUTTON_UPDATE_TIMER, sizeof(PIN_BUTTON_UPDATE_TIMER));
        printf("Button {UPDATE TIMER} was clicked!\n");
        delay(DELAY_TIME);
    }
}
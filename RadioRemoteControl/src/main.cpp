/** Libraries 
 * 
 *  TODO: 
 *      - ссылки на библиотеки
 *      - переделать путь для Settings
 */
#include <Arduino.h>
#include <printf.h>
#include <RF24.h>
#include <../../Libraries/Settings.h>


/** Define Pins 
 * 
 *  param: NRF
 *  param: Buttons
 */
#define PIN_NRF_CE  7
#define PIN_NRF_CSN 8

#define PIN_BUTTON_START                A5
#define PIN_BUTTON_STOP                 6

#define PIN_BUTTON_SCORE_GREEN_UP       A4
#define PIN_BUTTON_SCORE_GREEN_DOWN     A3

#define PIN_BUTTON_SCORE_RED_UP         2
// #define PIN_BUTTON_SCORE_RED_DOWN       3
#define PIN_BUTTON_UPDATE_SCORE         4
#define PIN_BUTTON_UPDATE_TIMER         5

#define DELAY_TIME 1000


/** Initialize Variables 
 */
// const uint8_t STATUS_BUTTON_START               = 0;
// const uint8_t STATUS_BUTTON_STOP                = 9;

// const uint8_t STATUS_BUTTON_SCORE_GREEN_UP      = 3;
// const uint8_t STATUS_BUTTON_SCORE_GREEN_DOWN    = 4;
// const uint8_t STATUS_BUTTON_SCORE_RED_UP        = 5;
// const uint8_t STATUS_BUTTON_SCORE_RED_DOWN      = 6;
// const uint8_t STATUS_BUTTON_UPDATE_SCORE        = 7;
// const uint8_t STATUS_BUTTON_UPDATE_TIMER        = 8;


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

    pinMode(PIN_BUTTON_START,               INPUT);
    pinMode(PIN_BUTTON_STOP,             INPUT);
    pinMode(PIN_BUTTON_SCORE_GREEN_UP,   INPUT);
    pinMode(PIN_BUTTON_SCORE_GREEN_DOWN, INPUT);
    pinMode(PIN_BUTTON_SCORE_RED_UP,     INPUT);
    // pinMode(STATUS_BUTTON_SCORE_RED_DOWN,   INPUT);
    pinMode(PIN_BUTTON_UPDATE_SCORE,     INPUT);
    pinMode(PIN_BUTTON_UPDATE_TIMER,     INPUT);

    // Wait for console opening
    delay(3000);
}


void remoteOperation(const uint8_t buttonOperation, char *buttonText)
{
    radio.write(&buttonOperation, sizeof(buttonOperation));
    printf("Button {%s} was clicked!\n", buttonText);
    delay(DELAY_TIME);
}

void loop() 
{
    if (digitalRead(PIN_BUTTON_START) == HIGH)                  remoteOperation(STATUS_BUTTON_START, "START");
    else if (digitalRead(PIN_BUTTON_STOP) == HIGH)              remoteOperation(STATUS_BUTTON_STOP, "STOP");
    else if (digitalRead(PIN_BUTTON_SCORE_GREEN_UP) == HIGH)    remoteOperation(STATUS_BUTTON_SCORE_GREEN_UP, "GREEN UP");
    else if (digitalRead(PIN_BUTTON_SCORE_GREEN_DOWN) == HIGH)  remoteOperation(STATUS_BUTTON_SCORE_GREEN_DOWN, "GREEN DOWN");
    else if (digitalRead(PIN_BUTTON_SCORE_RED_UP) == HIGH)      remoteOperation(STATUS_BUTTON_SCORE_RED_UP, "RED UP");
    // else if (digitalRead(PIN_BUTTON_SCORE_RED_DOWN) == HIGH)    remoteOperation(STATUS_BUTTON_SCORE_RED_DOWN, "RED DOWN");
    else if (digitalRead(PIN_BUTTON_UPDATE_SCORE) == HIGH)      remoteOperation(STATUS_BUTTON_UPDATE_SCORE, "UPDATE SCORE");
    else if (digitalRead(PIN_BUTTON_UPDATE_TIMER) == HIGH)      remoteOperation(STATUS_BUTTON_UPDATE_TIMER, "UPDATE TIMER");
}



/* -------------------------------------------------------------------------------------------------------------------------
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
*/

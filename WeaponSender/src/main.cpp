/** Libraries 
 * 
 *  TODO: 
 *      - ссылки на библиотеки
 *      - переделать путь для Settings
 */
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>


/** Define Pins 
 * 
 *  param: NRF
 *  param: Buttons
 *  param: LED
 */
#define PIN_NRF_CE  7
#define PIN_NRF_CSN 8

#define PIN_BUTTON 2

#define PIN_LED 4


/** Sportsmen Colors
 *  1 - GREEN
 *  2 - RED
 */
const uint8_t SPORTSMEN_COLOR = 1;


/** Initialize Objects 
 */
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);


// Setup
void setup() {

        // Serial settings
        Serial.begin(9600);
        printf_begin();
        printf("\n\r <<--- SENDER --->> \n\r");

        // Radio settings
        radio.begin();
        radio.setChannel(5);                      // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
        radio.setDataRate(RF24_1MBPS);            // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
        radio.setPALevel(RF24_PA_HIGH);           // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
        radio.openWritingPipe(0xAABBCCDD11LL);    // NANO
        // radio.openWritingPipe (0xAABBCCDD22LL);   // UNO

        // Wait for console opening
        delay(3000);
}


// TODO: Убрать светодиоды
// Loop
void loop() 
{
    /*
    Для шпаги, при нажатии кнопки, сигнал проходит до Sender'a
    */
    if (digitalRead(PIN_BUTTON) == HIGH)
    {
        radio.write(&SPORTSMEN_COLOR, sizeof(SPORTSMEN_COLOR));
        printf("Button {%d} clicked!\n", SPORTSMEN_COLOR);
        digitalWrite(PIN_LED, HIGH);
        delay(2000);
        digitalWrite(PIN_LED, LOW);
    }
}

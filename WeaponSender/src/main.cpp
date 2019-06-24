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
#include <../../Libraries/Settings.h>


/** Define Pins 
 * 
 *  param: Buttons
 *  param: LED
 *  param: NRF
 */
#define PIN_BUTTON 2

#define PIN_LED 4

#define PIN_NRF_CE  7
#define PIN_NRF_CSN 8


/** Define Variables 
 *  Константы переменных
 * 
 *  param: Delay
 */
#define DELAY_TIME_SEND_DATA 2000


/** Sportsmen Colors
 *  1 - GREEN
 *  2 - RED
 */
const uint8_t SPORTSMEN_COLOR = 1;


/** Initialize Objects 
 */
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);


/** Настройки serial порта 
 */
void serialSettings()
{
    Serial.begin(SERIAL_BAUDRATE);
    printf_begin();
    printf("[%ld] [MAIN] --- <<--- SENDER --->>\n", millis());
}


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
    radio.openWritingPipe(RADIO_ADDRESS_GREEN);
    radio.printDetails();
}

/** Отправка данных на контроллер
 */
void sendData () 
{
    radio.write(&SPORTSMEN_COLOR, sizeof(SPORTSMEN_COLOR));
    printf("[%ld] [INFO] --- Button {%d} clicked!\n", millis(), SPORTSMEN_COLOR);
    delay(DELAY_TIME_SEND_DATA);
}


/** Установка начальных значений 
 */
void setup()
{
    // Settings for serial port
    serialSettings();

    // Settings for radio channel
    radioSettings();

    // Wait for console opening
    delay(SERIAL_DELAY_CONSOLE_ACTIVATE);
}


/** Бесконечный цикл микроконтроллера 
 */
void loop() 
{
    /*
    Шпага: при нажатии кнопки, сигнал отправляется на контроллер 
    */
    if (digitalRead(PIN_BUTTON) == HIGH)
    {
        sendData();
    }
}

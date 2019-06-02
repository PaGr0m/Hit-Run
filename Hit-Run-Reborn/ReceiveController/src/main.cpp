/** TODO TASKS:
 *  Добавить кнопки?
 *      - для обновления счета
 *      - для обновления таймера
 * 
 *  Добавить таймер RTC3231
 *  Добавить .gitignore
 *  Добавить README
 */

/** Libraries 
 * 
 *  TODO: ссылки на библиотеки
 */
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <printf.h>


/** Define Pins 
 * 
 *  param: NRF
 *  param: LCD
 */
#define PIN_NRF_CE 7
#define PIN_NRF_CSN 8

#define PIN_LCD_DB7 3
#define PIN_LCD_DB6 4
#define PIN_LCD_DB5 5
#define PIN_LCD_DB4 6
#define PIN_LCD_RS  A0
#define PIN_LCD_E   A1


/** Define Variables 
 *  Константы переменных для функций
 * 
 *  param: SERIAL
 *  param: RADIO
 */
#define SERIAL_BAUDRATE 9600

#define RADIO_CHANNEL    5
#define RADIO_PIPE_GREEN 1
#define RADIO_PIPE_RED   2
#define RADIO_ADDRESS_GREEN 0xAABBCCDD11LL
#define RADIO_ADDRESS_RED   0xAABBCCDD22LL
#define RADIO_DATARATE RF24_1MBPS
#define RADIO_PALEVEL  RF24_PA_HIGH


/** Initialize Variables 
 */
const uint64_t pipe01 = 0xF0F1F2F3F4LL;
const uint8_t TIME_DUAL_HIT = 100;
uint8_t sportsmenGreen;
uint8_t sportsmenRed;
volatile boolean flag_tx = false;
volatile boolean flag_fail = false;
volatile boolean flag_rx = false;


/** Initialize Objects 
 */
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4, PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);


/** Настройки радиоканала 
 *  param: RADIO_CHANNEL --> Указываем канал приёма данных (от 0 до 127), 
 *                           5 - значит приём данных осуществляется на частоте 2,405 ГГц 
 *                           (на одном канале может быть только 1 приёмник и до 6 передатчиков)
 *  param: RADIO_DATARATE --> Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), 
 *                            RF24_1MBPS - 1Мбит/сек
 *  param: RADIO_PALEVEL --> Указываем мощность передатчика 
 *                           (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
 *  param: RADIO_PIPE_GREEN --> Труба спортсмена GREEN
 *  param: RADIO_PIPE_RED   --> Труба спортсмена RED
 *  param: RADIO_ADDRESS_GREEN --> Адрес спортсмена GREEN
 *  param: RADIO_ADDRESS_RED   --> Адрес спортсмена RED
 */
void radioSettings() 
{
    radio.begin();                                                  // Инициируем работу nRF24L01+
    radio.setChannel(RADIO_CHANNEL); 
    radio.setDataRate(RADIO_DATARATE);                     
    radio.setPALevel(RADIO_PALEVEL);                     
    radio.openReadingPipe(RADIO_PIPE_GREEN, RADIO_ADDRESS_GREEN);   // Открываем трубу с идентификатором, для приема данных
    radio.openReadingPipe(RADIO_PIPE_RED, RADIO_ADDRESS_RED);       // Открываем трубу с идентификатором, для приема данных
    radio.printDetails();
    radio.startListening();
}

// TODO: возможно стоит переделать на tx_ok, tx_fail, rx_ready
/** Обработчик прерывания 
 *  param: flag_tx   --> 1 если успешно отправили данные
 *  param: flag_fail --> 1 если данные не отправленны
 *  param: flag_rx   --> 1 если пришли данные для чтения
 */
void checkRadioData() 
{
    bool tx, fail, rx;
    radio.whatHappened(tx, fail, rx);
    flag_tx = tx;               
    flag_fail = fail;            
    flag_rx = rx;             
}

// TODO: добавить таймер
/** Шаблон дисплея 
 *  Визуализация шаблона на дисплее
 */
void displayTemplate() 
{
    lcd.setCursor(0, 0);
    lcd.print("GREEN  0:0  RED ");

    lcd.setCursor(0, 1);
    lcd.print("                ");
}

/** Инициализия символов на дисплее 
 *  param: RAW     --> первый параметр setCursor
 *  param: COLLUMN --> второй параметр setCursor
 */
void displayInitial() 
{
    lcd.begin(2, 16);

    lcd.setCursor(0, 0);
    lcd.print("   Controller   ");

    lcd.setCursor(0, 1);
    lcd.print("by Pavel Gromov");

    // Wait for Serial Console opening
    delay(3000);

    displayTemplate();
}

/** Обновление счетчика на дисплее 
 *  param: sportsmenGreen --> счетчик уколов зеленого спортсмена
 *  param: sportsmenRed   --> счетчик уколов красного спортсмена
 */
void updateScoreLcd(uint8_t sportsmenGreen, uint8_t sportsmenRed)
{
    lcd.setCursor(sportsmenGreen < 10 ? 7 : 6, 0);
    lcd.print(sportsmenGreen);

    lcd.setCursor(9, 0);
    lcd.print(sportsmenRed);
}

/** Обновление счетчика уколов 
 *  TODO: ОПИСАНИЕ СТАТУСОВ
 *  param: status 
 *      --> 1 - укол присуждается зеленому спортсмену
 *      --> 2 - укол присуждается красному спортсмену
 *      --> 3 - укол присуждается обоим спортсменам
 */
void updateScoreCounter(uint8_t status) 
{
    if (status == 1)
    {
        sportsmenGreen++;
    }
    else if (status == 2)
    {
        sportsmenRed++;
    }
    else if (status == 3)
    {
        sportsmenGreen++;
        sportsmenRed++;
    }

    updateScoreLcd(sportsmenGreen, sportsmenRed);
}

/** Установка начальных значений 
 */
void setup()
{
    /** Прерывание 
     *  При "падающем" прерывании на pin 2 (int 0) 
     *  вызывается функция checkRadioData
     */
    attachInterrupt(0, checkRadioData, FALLING);

    Serial.begin(SERIAL_BAUDRATE);
    printf_begin();
    printf("\n\r >>>>> RECEIVER-CONTROLLER <<<<< \n\r");

    // Radio settings
    radioSettings();

    // LCD initial    
    displayInitial();
}

/** Бесконечный цикл микроконтроллера 
 */
void loop() 
{
    // Если пришли данные для чтения

    if (flag_rx) 
    {
        printf("Flag_RX: true \n"); // TODO: убрать
        flag_rx = false;

        uint8_t sportsmenStatus = 0;
        radio.read(&sportsmenStatus, sizeof(sportsmenStatus));

        // FIXME: попытка проверки обоюдных ударов
        uint8_t beginTime = millis();
        while (millis() < beginTime + 40)
        {
            if (flag_rx)
            {
                flag_rx = false;
                sportsmenStatus = 3;   
            } 
        }
        // ---------------------------------------

        printf("Sportsmen hit status: %d \n", sportsmenStatus);
        updateScoreCounter(sportsmenStatus);
    }

    /** Если данные не отправленны 
     * param: flag_fail --> меняем флаг
     */
    if (flag_fail)
    {
        printf("Flag_fail : данные не отправлены \n");
        flag_fail = 0;
    }

    /** Если данные отправленны 
     * param: flag_tx --> меняем флаг
     */
    if (flag_tx)
    {
        printf("Flag_tx : данные отправлены \n");
        flag_tx = 0;
    }
}

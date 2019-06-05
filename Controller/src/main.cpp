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
#include <RTClib.h>
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

#define LCD_COLUMN_TIMER_MINUTE 7
#define LCD_COLUMN_TIMER_SECONDS 9
#define LCD_ROW_TIMER 1

/** Initialize Variables 
 */
uint8_t sportsmenGreen;
uint8_t sportsmenRed;
volatile bool flag_tx = false;
volatile bool flag_fail = false;
volatile bool flag_rx = false;

// const uint8_t STATUS_SPORTSMEN_GREEN            = 1;
// const uint8_t STATUS_SPORTSMEN_RED              = 2;
// const uint8_t STATUS_BUTTON_START               = 0;
// const uint8_t STATUS_BUTTON_SCORE_GREEN_UP      = 3;
// const uint8_t STATUS_BUTTON_SCORE_GREEN_DOWN    = 4;
// const uint8_t STATUS_BUTTON_SCORE_RED_UP        = 5;
// const uint8_t STATUS_BUTTON_SCORE_RED_DOWN      = 6;
// const uint8_t STATUS_BUTTON_UPDATE_SCORE        = 7;
// const uint8_t STATUS_BUTTON_UPDATE_TIMER        = 8;
// const uint8_t STATUS_BUTTON_STOP                = 9;

const uint8_t STATUS_HIT_GREEN          = 1;
const uint8_t STATUS_HIT_RED            = 2;
const uint8_t STATUS_HIT_DUAL           = 3;
const uint8_t STATUS_HIT_GREEN_REMOVE   = 8;
const uint8_t STATUS_HIT_RED_REMOVE     = 9;
const uint8_t STATUS_HIT_UPDATE         = 0;


//---
const char ROUND_MINUTE = 2;
const char ROUND_SECONDS = 60;

uint8_t roundMinute = 0;
uint8_t roundSecond = 0;
//--- 



/** Initialize Objects 
 */
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4, PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);
RTC_DS3231 rtc;


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
    lcd.print("Timer  3:00     ");
}

/** Инициализия символов на дисплее 
 *  param: RAW     --> первый параметр setCursor
 *  param: COLLUMN --> второй параметр setCursor
 */
void displayInitial() 
{
    lcd.begin(2, 16);

    lcd.setCursor(0, 0);
    lcd.print("Controller V2.1");

    lcd.setCursor(0, 1);
    lcd.print(" by Pavel Gromov");

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
    if (status == STATUS_HIT_GREEN)
    {
        sportsmenGreen++;
    }
    else if (status == STATUS_HIT_RED)
    {
        sportsmenRed++;
    }
    else if (status == STATUS_HIT_DUAL)
    {
        sportsmenGreen++;
        sportsmenRed++;
    }
    else if (status == STATUS_HIT_GREEN_REMOVE)
    {
        if (sportsmenGreen > 0) sportsmenGreen--;
    }
    else if (status == STATUS_HIT_RED_REMOVE)
    {
        if (sportsmenGreen > 0) sportsmenRed--;
    }
    else if (status == STATUS_HIT_UPDATE)
    {
        sportsmenRed = 0;
        sportsmenGreen = 0;
    }

    updateScoreLcd(sportsmenGreen, sportsmenRed);
}

void initialTimerLcd()
{
    lcd.setCursor(0, 1);
    lcd.print("Timer  3:00     ");
    
    roundMinute = 0;
    roundSecond = 0;
}

void updateTimerLcd(uint8_t minute, uint8_t seconds)
{
    if (seconds != 60) 
    {
        lcd.setCursor(LCD_COLUMN_TIMER_MINUTE, LCD_ROW_TIMER);
        lcd.print(minute);

        lcd.setCursor(LCD_COLUMN_TIMER_SECONDS, LCD_ROW_TIMER);
        lcd.print(seconds);
    }
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

    // rtc.begin();
    // rtc.adjust(DateTime(-1, -1, -1, -1, 0, 0));

    rtc.begin();
    if (rtc.lostPower()) 
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}


// TODO: что будет, если read ничего не вернет ?
// TODO: проверить на работоспособность. Иначе переделать прерывание
/** Бесконечный цикл микроконтроллера 
 */
void loop() 
{
    // Если пришли данные для чтения

    if (flag_rx) 
    {
        printf("Flag_RX: true \n"); // TODO: убрать
        flag_rx = false;

        uint8_t receiveStatus = 1000;
        radio.read(&receiveStatus, sizeof(receiveStatus));
        printf("Status: %d \n", receiveStatus); // TODO: убрать


        // не уверен, что будет работать
        // возможно стоит изменить прерывание и уже там принимать переменную, а тут просто проверять ее в условия, и обновлять на несуществующий статус
        if (receiveStatus == STATUS_BUTTON_START)
        {
            rtc.adjust(DateTime(-1, -1, -1, -1, roundMinute, roundSecond));

            while (flag_rx == false)
            {
                // printf("Fight | time: %d \n", millis());
                printf("Fight | minute: %d, seconds: %d \n", rtc.now().minute(), rtc.now().second());
                if (flag_rx)
                {
                    flag_rx = false;

                    uint8_t receiveStatus = 0;
                    radio.read(&receiveStatus, sizeof(receiveStatus));

                    if (receiveStatus == STATUS_SPORTSMEN_GREEN || receiveStatus == STATUS_SPORTSMEN_RED)
                    {
                        // FIXME: попытка проверки обоюдных ударов
                        uint8_t beginTime = millis();
                        while (millis() < beginTime + 40)
                        {
                            if (flag_rx) // TODO: проверка какой статус пришел
                            {
                                flag_rx = false;
                                receiveStatus = 3;   
                            } 
                        }
                        // ---------------------------------------

                        printf("Sportsmen hit status: %d \n", receiveStatus);
                        updateScoreCounter(receiveStatus);

                        roundMinute = rtc.now().minute();
                        roundSecond = rtc.now().second();

                        break;
                    }
                    else if (receiveStatus == STATUS_BUTTON_STOP)
                    {
                        printf("!!! Stop !!!");
                        
                        roundMinute = rtc.now().minute();
                        roundSecond = rtc.now().second();

                        break;
                    }
                }
                updateTimerLcd(ROUND_MINUTE - rtc.now().minute(), ROUND_SECONDS - rtc.now().second());
            } 
        }
        else if (receiveStatus == STATUS_SPORTSMEN_GREEN || receiveStatus == STATUS_SPORTSMEN_RED)
        {
            flag_rx = false;

            // FIXME: попытка проверки обоюдных ударов
            uint8_t beginTime = millis();
            while (millis() < beginTime + 40)
            {
                if (flag_rx)
                {
                    flag_rx = false;
                    receiveStatus = 3;   
                } 
            }
            // ---------------------------------------

            printf("[Test] --> Sportsmen hit status: %d \n", receiveStatus);
        }
        else if (receiveStatus == STATUS_BUTTON_SCORE_GREEN_UP) updateScoreCounter(STATUS_HIT_GREEN);
        else if (receiveStatus == STATUS_BUTTON_SCORE_GREEN_DOWN) updateScoreCounter(STATUS_HIT_GREEN_REMOVE);
        else if (receiveStatus == STATUS_BUTTON_SCORE_RED_UP) updateScoreCounter(STATUS_HIT_RED);
        else if (receiveStatus == STATUS_BUTTON_SCORE_RED_DOWN) updateScoreCounter(STATUS_HIT_RED_REMOVE);
        else if (receiveStatus == STATUS_BUTTON_UPDATE_SCORE) updateScoreCounter(STATUS_HIT_UPDATE);
        else if (receiveStatus == STATUS_BUTTON_UPDATE_TIMER) initialTimerLcd();
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

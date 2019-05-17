#include <Arduino.h>

#include <SPI.h>

#include <LiquidCrystal.h>
#include <IRremote.h>
#include "printf.h"

#define REMOTE_CONTROL_CODE_START 0xFFA857
#define REMOTE_CONTROL_CODE_STOP 0xFFA858

#define REMOTE_CONTROL_CODE_REFRESH_TIMER 0xFFA857


#define PIN_IR_REMOTE_CONTROL 12
#define PIN_LCD_RS  7
#define PIN_LCD_E   6
#define PIN_LCD_DB4 5
#define PIN_LCD_DB5 4
#define PIN_LCD_DB6 3
#define PIN_LCD_DB7 2

// Initialize
IRrecv irRecv(PIN_IR_REMOTE_CONTROL);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4,
                  PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);
RTC_DS3231 rtc;

decode_results results;

byte data;
byte sportsmenFirst  = 0;
byte sportsmenSecond = 0;



// TODO: заменить столбцы на переменные
/*
Метод обновляет счет бойцов на экране LCD
*/
void updateLcdScore(byte first, byte second)
{
    lcd.setCursor(first < 10 ? 5 : 4, 0);
    lcd.print(first);

    lcd.setCursor(7, 0);
    lcd.print(sportsmenRed);
}

/*
Метод обновляет таймер на экране LCD
*/
void updateLcdTimer(uint8_t minute, uint8_t second)
{
    lcd.setCursor(first < 10 ? 5 : 4, 0);
    lcd.print(first);

    lcd.setCursor(7, 0);
    lcd.print(sportsmenRed);
}

/*
Метод инициализации при запуске микроконтроллера
*/
void setup()
{
    Serial.begin(9600);
    printf_begin();
    printf("\n\r ***** CONTROLLER ****** \n\r");

    // Wait for console opening
    delay(3000);
}

/*
Метод работающий в бесконечном цикле
*/
void loop()
{
    /*
    Проверяем, какая команда пришла с пульта ДУ
    */
    if (irRecv.decode(&results))
    {
        switch (results.value)
        {
            // Начать бой
            case REMOTE_CONTROL_CODE_START:
            {
                /*
                Бой идет пока не нажата кнопка STOP или не был произведен укол
                */

                // -- Тестовая часть с ПДУ --
                irRecv.resume();
                irRecv.decode(&results);
                // ------

                rtc.adjust(DateTime(-1, -1, -1, 0, roundMinute, roundSecond));
                // while (!REMOTE_CONTROL_CODE_STOP)
                while ( )
                {
                    // Проверка:
                    // были ли присланы данные от спортсменов об уколах
                    if (Serial.available())
                    {
                        data = Serial.read();

                        if (char(data) == '1') sportsmenFirst++;
                        if (char(data) == '2') sportsmenSecond++;

                        if (char(data) == '3')
                        {
                            sportsmenFirst++;
                            sportsmenSecond++;
                        }


                        lcd.setCursor(0, 1);
                        // lcd.write(data);
                        lcd.print(char(data));

                        roundMinute = rtc.now().minute();
                        roundSecond = rtc.now().second();

                        break;
                    }
                    // Есди данных нет, продолжаем таймер
                    else
                    {
                        // TODO: Обновить таймер

                        // TODO: заменить строки/столбы на переменные
                        lcd.setCursor(5, 1);
                        lcd.print(ROUND_MINUTE - rtc.now().minute());
                        lcd.setCursor(7, 1);
                        lcd.print(60 - rtc.now().second());
                    }
                }
                updateLcdScore(&sportsmenFirst, &sportsmenSecond);

                break;
            }

            // Обновить время на таймере до 3:00
            case REMOTE_CONTROL_CODE_REFRESH_TIMER:
            {
                roundMinute = ROUND_MINUTE;
                roundSecond = 0;
                // rtc.adjust(DateTime(-1, -1, -1, 0, roundMinute, roundSecond));

                // TODO: вывод на LCD таймер (вручную)
            }

            case REMOTE_CONTROL_CODE_REFRESH_SCORE:
            {
                sportsmenFirst  = 0;
                sportsmenSecond = 0;

                updateLcdScore(&sportsmenFirst, &sportsmenSecond);

                // TODO: вывод на LCD счёта (вручную)
            }
        }

        irRecv.resume();
    }


    // if (Serial.available())
    // {
    //     data = Serial.read();
    //
    //     lcd.setCursor(0, 1);
    //     // lcd.write(data);
    //     lcd.print(char(data));
    // }
}

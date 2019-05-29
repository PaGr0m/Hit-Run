#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal.h>

const uint64_t pipe01 = 0xF0F1F2F3F4LL;
volatile boolean flag_tx;
volatile boolean flag_fail;
volatile boolean flag_rx;

int i;

RF24 radio(9, 10);
LiquidCrystal lcd(15, 16, 2);

void setup()
{
    /**
     * При "падающем" прерывании на pin 3 
     * вызывается функция check_radio
     */
    attachInterrupt(1, check_radio, FALLING);

    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    radio.begin();

    delay(2);
    radio.setChannel(9); // канал (0-127)
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_HIGH);

    radio.openReadingPipe(1, pipe01);
    radio.startListening();
}

// обработчик прерывания
void check_radio()
{
    bool tx, fail, rx;
    radio.whatHappened(tx, fail, rx); // читаем регистр
    flag_tx = tx;                     // 1 если успешно отправили данные
    flag_fail = fail;                 // 1 если данные не отправленны
    flag_rx = rx;                     // 1 если пришли данные для чтения
}

void loop()
{
    // если пришли данные для чтения
    if (flag_rx)
    {
        float temperature = 0; // забераем их и выводим на экран
        radio.read(&temperature, sizeof(temperature));

        lcd.setCursor(0, 0);
        if (temperature > 0)
            lcd.print("+");
        lcd.print(temperature);
        lcd.print("   ");
        //       Serial.println(temperature);
        //       Serial.println();
        flag_rx = 0;
    }

    // если данные не отправленны
    if (flag_fail)
    {
        flag_fail = 0;
    }

    // если данные отправленны
    if (flag_tx)
    {
        flag_tx = 0;
    }

    ///////////////////
    // можно занятся другими делами.
    lcd.setCursor(0, 1);
    i++;
    lcd.print(i);
    delay(250);
}
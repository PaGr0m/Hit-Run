#include <Arduino.h>

#include <SPI.h>

#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define PIN_NRF_CE  7
#define PIN_NRF_CSN 8

#define PIN_LED 2
#define PIN_BUTTON 4




// Initialize
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);

byte buttonState = 0;

byte blankMessage = 9;
// byte blankMessage = NULL;

void send() {
        buttonState = digitalRead(PIN_BUTTON);

        if (buttonState == LOW)
        {
                radio.write(blankMessage, sizeof(blankMessage));
                // digitalWrite(PIN_LED, HIGH);
                delay(2000);
                // digitalWrite(PIN_LED, LOW);
        }
}
// Setup
void setup() {

        // Serial settings
        Serial.begin(9600);
        printf_begin();

        // Radio settings
        radio.begin();
        radio.setChannel(5);                      // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
        radio.setDataRate(RF24_1MBPS);            // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
        radio.setPALevel(RF24_PA_HIGH);           // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
        radio.openWritingPipe(0xAABBCCDD11LL);    // NANO
        // radio.openWritingPipe (0xAABBCCDD22LL);   // UNO

}

// Loop
void loop() {

        /*
           Для шпаги, при нажатии кнопки, сигнал проходит до Sender'a
         */
        // buttonState = digitalRead(PIN_BUTTON);
        //
        // if (buttonState == LOW)
        // {
        //         Serial.print("Button\n");
        //         counter++;
        //         radio.write(&counter, sizeof(counter));
        //         printf("%d\n", counter);
        //         digitalWrite(PIN_LED, HIGH);
        //         delay(2000);
        //         digitalWrite(PIN_LED, LOW);
        // }

        send();
}

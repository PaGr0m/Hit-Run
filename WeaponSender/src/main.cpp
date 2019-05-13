#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"


#define PIN_BUTTON 4
#define PIN_LED 2

// WITH ADAPTER
// #define CE_PIN  9
// #define CSN_PIN 10
// END WITH

// WITHOUT ADAPTER
#define CE_PIN  7
#define CSN_PIN 8


// Initialize
RF24 radio(CE_PIN, CSN_PIN);
byte counter = 0;
byte buttonState = 0;
// END

void setup()
{
  // <--- SERIAL SETTINGS --->
  Serial.begin(9600);
  printf_begin();
  printf("\n\r Address: 0xAABBCCDD22LL, sportsmen - GREEN \n\r");
  printf("\n\r <----- EPEE SENDER -----> \n\r");


  // <--- RADIO SETTINGS --->
  radio.begin();

  radio.setChannel(5);                                       // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_1MBPS);                        // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);                      // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openWritingPipe (0xAABBCCDD11LL);                    // NANO
  // radio.openWritingPipe (0xAABBCCDD22LL);                    // UNO

  // radio.stopListening();

  // BUTTON SETTINGS
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // LED SETTINGS
  pinMode(PIN_LED, OUTPUT);

}

void loop(void)
{

  /********************** SENDER *******************************/
  buttonState = digitalRead(PIN_BUTTON);
  if (buttonState == LOW)
  {
    Serial.print("Button\n");
    counter++;
    radio.write(&counter, sizeof(counter));
    printf("%d\n", counter);
    digitalWrite(PIN_LED, HIGH);
    delay(2000);
    digitalWrite(PIN_LED, LOW);
  }
}

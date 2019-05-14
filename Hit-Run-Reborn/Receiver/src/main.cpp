#include <Arduino.h>

#include <SPI.h>

#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define PIN_NRF_CSN 10
#define PIN_NRF_CE 9

// Initialize
RF24 radio(CE_PIN, CSN_PIN);

// Setup
void setup() {
  Serial.begin(9600);
  printf_begin();

  // Radio settings
  radio.begin();                              // Инициируем работу nRF24L01+
  radio.setChannel(5);                        // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate(RF24_1MBPS);              // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel(RF24_PA_HIGH);             // Указываем мощность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openReadingPipe(1, 0xAABBCCDD11LL);   // Открываем 1 трубу с идентификатором 1 передатчика 0xAABBCCDD11, для приема данных
  radio.openReadingPipe(2, 0xAABBCCDD22LL);   // Открываем 2 трубу с идентификатором 2 передатчика 0xAABBCCDD22, для приема данных
  radio.printDetails();
  radio.startListening();

  // Wait for console opening
  delay(3000);
}

// Loop
void loop() {

  /*
  Если в буфере имеются принятые данные, то получаем номер трубы,
  по которой они пришли, по ссылке на переменную pipe
  */
  if(radio.available(&pipe))
  {
    Serial.printf("%s\n", pipe);

    // TODO: добавить возможность обоюдного удара
    // дать время на считывание второго спортсмена
  }

}

#include <SPI.h>
#include <LiquidCrystal.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define CE_PIN  9
#define CSN_PIN 10
#define LED_PIN 7
#define BUZZER_PIN 4
#define TONE 500

#define PIN_LCD_RS  7
#define PIN_LCD_E   6
#define PIN_LCD_DB4 5
#define PIN_LCD_DB5 4
#define PIN_LCD_DB6 3
#define PIN_LCD_DB7 2

// Initialize
RF24 radio(CE_PIN, CSN_PIN);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4, PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7); // (RS, E, DB4, DB5, DB6, DB7)
byte counter = 1;
// const uint64_t pipe = 0xE8E8F0F0E1LL;

// const byte addresses[2][6] = {
//   "00001",
//   "00002"
// };
// const byte Addr1[6] = "00001";
// const byte Addr2[6] = "00002";

int            data[1];  // Создаём массив для приёма данных (так как мы будем принимать от каждого передатчика только одно двухбайтное число, то достаточно одного элемента массива типа int)
uint8_t        pipe;

byte sportsmenRed;
byte sportsmenGreen;
byte column;

boolean flag = true;

void setup()
{
  // <--- SERIAL SETTINGS: BEGIN --->
  Serial.begin(9600);
  printf_begin();
  printf("\n\r ***** REMOTE CONTROL ***** \n\r");
  // printf("%s", Addr1);
  // <--- SERIAL SETTINGS: END --->


  // <--- RADIO SETTINGS: BEGIN --->
  // radio.begin();
  // radio.setAutoAck(1);                    // Ensure autoACK is enabled
  // radio.enableAckPayload();               // Allow optional ack payloads
  // radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  // radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  //
  // // radio.openReadingPipe(1, pipe);      // Open a reading pipe on address 0, pipe 1
  // radio.startListening();                 // Start listening
  // radio.powerUp();
  // radio.printDetails();                   // Dump the configuration of the rf unit for debugging


  // <--- 2 BEGIN --->
  // radio.setPALevel(RF24_PA_MIN);
  // radio.setChannel(108);
  // radio.setDataRate( RF24_250KBPS );
  // <--- 2 END --->

  // <--- 3 BEGIN --->
  radio.begin();                                             // Инициируем работу nRF24L01+
  radio.setChannel(5);                                       // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_1MBPS);                        // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);                      // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openReadingPipe (1, 0xAABBCCDD11LL);                 // Открываем 1 трубу с идентификатором 1 передатчика 0xAABBCCDD11, для приема данных
  radio.openReadingPipe (2, 0xAABBCCDD22LL);                 // Открываем 2 трубу с идентификатором 2 передатчика 0xAABBCCDD22, для приема данных
  radio.startListening  ();
  // <--- 3 END --->


  // <--- RADIO SETTINGS: END --->

  // <--- LED DISPLAY: BEGIN --->
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("RED  0:0  GREEN");

  lcd.setCursor(0, 1);
  lcd.print("Round 1");
  // <--- LED DISPLAY: END --->


  // BUZZER SETTINGS
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop(void)
{

/********************* RECEIVER ******************************/
    // <--- FIRST BEGIN -->
    // byte pipeNo, gotByte;                           // Declare variables for the pipe and the byte received
    // while( radio.available(&pipeNo))                // Read all available payloads
    // {
    //   radio.read( &gotByte, 1 );
    //
    //   radio.writeAckPayload(pipeNo, &gotByte, 1 );  // This can be commented out to send empty payloads.
    //   printf("Sent response %d \n\r", gotByte);
    // }
    // <--- FIRST END --->


    //// <--- SECOND BEGIN --->
    // radio.openReadingPipe(0, Addr1);
    // radio.startListening();
    // if (radio.available())
    // {
    //   flag = true;
    //   radio.read(&sportsmenRed, sizeof(sportsmenRed));
    //   radio.stopListening();
    // }
    // delay(10);
    //
    // radio.openReadingPipe(0, Addr2);
    // radio.startListening();
    // if (radio.available())
    // {
    //   flag = true;
    //   radio.read(&sportsmenGreen, sizeof(sportsmenGreen));
    //   radio.stopListening();
    // }
    // delay(10);
    //
    // if (flag)
    //   printf("RED %d : %d GREEN\n", sportsmenRed, sportsmenGreen);
    //
    // flag = false;
    //
    // // -----
    // if (sportsmenRed < 10)
    //   column = 5;
    // else
    //   column = 4;
    //
    // lcd.setCursor(column, 0);
    // lcd.print(sportsmenRed);
    //
    // lcd.setCursor(7, 0);
    // lcd.print(sportsmenGreen);
    //// <--- SECOND END --->


    // <--- THIRD BEGIN --->
    if(radio.available(&pipe))  // Если в буфере имеются принятые данные, то получаем номер трубы, по которой они пришли, по ссылке на переменную pipe
    {
        // Читаем данные в массив data и указываем сколько байт читать
        if(pipe == 1)
          radio.read(&sportsmenRed, sizeof(sportsmenRed));
        if(pipe == 2)
          radio.read(&sportsmenGreen, sizeof(sportsmenGreen));

        printf("PIPE: %d\n", pipe);
        printf("RED %d : %d GREEN\n", sportsmenRed, sportsmenGreen);

        if (sportsmenRed < 10)
          column = 5;
        else
          column = 4;

        lcd.setCursor(column, 0);
        lcd.print(sportsmenRed);

        lcd.setCursor(7, 0);
        lcd.print(sportsmenGreen);

        lcd.setCursor(10, 1);
        lcd.print("PIPE");
        lcd.setCursor(15, 1);
        lcd.print(pipe);
    }


    // <--- THIRD END --->
}

void beepAndLed ()
{
  digitalWrite(LED_PIN, HIGH);
  tone (BUZZER_PIN, TONE);
  delay(1000);
  noTone(BUZZER_PIN);
  digitalWrite(LED_PIN, LOW);
}

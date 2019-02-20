#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define CE_PIN  9
#define CSN_PIN 10
#define LED_PIN 7
#define BUZZER_PIN 4
#define TONE 500

// Initialize
RF24 radio(CE_PIN, CSN_PIN);
byte counter = 1;
// const uint64_t pipe = 0xE8E8F0F0E1LL;

const byte addresses[2][6] = {
  "00001",
  "00002"
}
byte sportsmenRed;
byte sportsmenGreen;


void setup()
{
  // SERIAL SETTINGS
  Serial.begin(9600);
  printf_begin();
  printf("\n\r ***** REMOTE CONTROL ***** \n\r");

  // RADIO SETTINGS
  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed

  // radio.openReadingPipe(1, pipe);      // Open a reading pipe on address 0, pipe 1
  radio.startListening();                 // Start listening
  radio.powerUp();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging


  // <--- BEGIN --->
  // radio.setPALevel(RF24_PA_MIN);
  // radio.setChannel(108);
  // radio.setDataRate( RF24_250KBPS );
  // <--- END --->

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

    // <--- SECOND BEGIN --->
    radio.openReadingPipe(0, addresses[0]);
    radio.startListening();
    if (radio.available())
    {
      radio.read(&sportsmenRed, 1);
      radio.stopListening();
    }
    delay(100);

    radio.openReadingPipe(0, Addr2);
    radio.startListening();
    if (radio.available())
    {
      radio.read(&tmp2, 1);
      radio.stopListening();
    }
    delay(100);

    Serial.print(sportsmenRed);
    Serial.print(", ");
    Serial.println(sportsmenGreen);
    // <--- SECOND END --->
}

void beepAndLed ()
{
  digitalWrite(LED_PIN, HIGH);
  tone (BUZZER_PIN, TONE);
  delay(1000);
  noTone(BUZZER_PIN);
  digitalWrite(LED_PIN, LOW);
}

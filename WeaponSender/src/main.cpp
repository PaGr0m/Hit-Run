#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define CE_PIN  9
#define CSN_PIN 10
#define PIN_BUTTON 7
#define PIN_LED 4

// Initialize
RF24 radio(CE_PIN, CSN_PIN);
byte counter = 1;
// const uint64_t pipe = 0xE8E8F0F0E1LL;
int buttonState = 0;
// END

// TEST
byte tmp = 0;
// const byte address[6] = "00001"; // for Arduino UNO
const byte address[6] = "00002"; // for Arduino NANO

void setup()
{
  // SERIAL SETTINGS
  Serial.begin(9600);
  printf_begin();
  printf("\n\r Address: 0xAABBCCDD22LL, sportsmen - GREEN \n\r");
  printf("\n\r <----- EPEE SENDER -----> \n\r");

  // RADIO SETTINGS
  radio.begin();
  // radio.setAutoAck(1);                    // Ensure autoACK is enabled
  // radio.enableAckPayload();               // Allow optional ack payloads
  // radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  // radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  // radio.openWritingPipe(pipe);            // Both radios listen on the same pipes by default, and switch when writing
  // radio.startListening();                 // Start listening
  // radio.powerUp();
  // radio.printDetails();                   // Dump the configuration of the rf unit for debugging

  // // <--- 2 BEGIN --->
  // radio.setPALevel(RF24_PA_MIN);
  // radio.setChannel(108);
  // radio.setDataRate(RF24_250KBPS);
  // radio.setRetries(1, 15);
  // radio.openWritingPipe(address);
  //
  // radio.stopListening();
  // // <--- 2 END --->

  // <--- 3 BEGIN --->
  radio.setChannel(5);                                       // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_1MBPS);                        // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);                      // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  // radio.openWritingPipe (0xAABBCCDD11LL);                    // NANO
  radio.openWritingPipe (0xAABBCCDD22LL);                    // UNO

  // radio.stopListening();
  // <--- 3 END --->

  // BUTTON SETTINGS
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // LED SETTINGS
  pinMode(PIN_LED, OUTPUT);

}

void loop(void)
{

/********************** SENDER *******************************/
buttonState = digitalRead(PIN_BUTTON);
if (buttonState == HIGH)
{
  Serial.print("Button\n");
  tmp++;
  radio.write(&tmp, sizeof(tmp));
  printf("%d\n", tmp);
  digitalWrite(PIN_LED, HIGH);
  delay(2000);
}
digitalWrite(PIN_LED, LOW);

// if (buttonState == HIGH)
// {
//
//     byte gotByte;                                           // Initialize a variable for the incoming response
//
//     radio.stopListening();                                  // First, stop listening so we can talk.
//     printf("Now sending %d as payload. ",counter);          // Use a simple byte counter as payload
//     unsigned long time = micros();                          // Record the current microsecond count
//
//     if (radio.write(&counter, 1))
//     {                                                       // Send the counter variable to the other radio
//         if(!radio.available())                              // If nothing in the buffer, we got an ack but it is blank
//             printf("Got blank response. round-trip delay: %lu microseconds\n\r", micros()-time );
//         else
//             while(radio.available() ){                      // If an ack with payload was received
//                 radio.read( &gotByte, 1 );                  // Read it, and display the response time
//                 printf("Got response %d, round-trip delay: %lu microseconds\n\r", gotByte, micros()-time);
//                 counter++;                                  // Increment the counter variable
//             }
//     }
//     else                                                    // If no ack response, sending failed
//       printf("Sending failed.\n\r");
//
//       // Try again later
//       delay(1000);
//   }
}

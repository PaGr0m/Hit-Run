#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define CE_PIN  9
#define CSN_PIN 10
#define BUTTON_PIN 2

// Initialize
RF24 radio(CE_PIN, CSN_PIN);
byte counter = 1;
const uint64_t pipe = 0xE8E8F0F0E1LL;
int buttonState = 0;
// END


void setup()
{
  // SERIAL SETTINGS
  Serial.begin(9600);
  printf_begin();
  printf("\n\r <----- EPEE SENDER -----> \n\r");

  // RADIO SETTINGS
  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipe);            // Both radios listen on the same pipes by default, and switch when writing
  radio.startListening();                 // Start listening
  radio.powerUp();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging

  // BUTTON SETTINGS
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop(void)
{

/********************** SENDER *******************************/
buttonState = digitalRead(BUTTON_PIN);
if (buttonState == HIGH) {

    byte gotByte;                                           // Initialize a variable for the incoming response

    radio.stopListening();                                  // First, stop listening so we can talk.
    printf("Now sending %d as payload. ",counter);          // Use a simple byte counter as payload
    unsigned long time = micros();                          // Record the current microsecond count

    if (radio.write(&counter, 1))
    {                                                       // Send the counter variable to the other radio
        if(!radio.available())                              // If nothing in the buffer, we got an ack but it is blank
            printf("Got blank response. round-trip delay: %lu microseconds\n\r", micros()-time );
        else
            while(radio.available() ){                      // If an ack with payload was received
                radio.read( &gotByte, 1 );                  // Read it, and display the response time
                printf("Got response %d, round-trip delay: %lu microseconds\n\r", gotByte, micros()-time);
                counter++;                                  // Increment the counter variable
            }
    }
    else                                                    // If no ack response, sending failed
      printf("Sending failed.\n\r");

      // Try again later
      delay(1000);
  }
}

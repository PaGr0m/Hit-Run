#include <Arduino.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define pinButton 4

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);
                                                                           // Topology
byte addresses[][6] = {"1Node","2Node"};              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;                                              // The role of the current running sketch

byte counter = 1;                                                          // A single byte to keep track of the data being sent back and forth
byte cnt = 5;

void setup() {
  pinMode(pinButton, INPUT_PULLUP);

  Serial.begin(9600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r", role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // Setup and configure radio

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(addresses[1]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
  radio.startListening();                 // Start listening
  radio.powerUp();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void) {

/***************/
  boolean button = !digitalRead(pinButton);

    if (role == role_ping_out)
    {
        if (button)
        {
          byte gotByte;                                           // Initialize a variable for the incoming response

          radio.stopListening();                                  // First, stop listening so we can talk.
          printf("Now sending %d as payload. ",counter);          // Use a simple byte counter as payload
          unsigned long time = micros();

          if (radio.write(&cnt, 1))
          {
              if(!radio.available())
              {
                  printf("Got blank response. round-trip delay: %lu microseconds\n\r",micros()-time);
              }
              else
              {
                  while(radio.available())
                  {
                      radio.read( &gotByte, 1 );
                      printf("Got response %d, round-trip delay: %lu microseconds\n\r",gotByte,micros()-time);
                      cnt++;
                  }
              }
        }
        else
        {
          printf("Sending failed.\n\r");
        }

        delay(1000);
      }
  }

/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      role = role_ping_out;                      // Change roles (ping out)
      radio.openWritingPipe(addresses[0]);       // Open different pipes when writing. Write on pipe 0, address 0
      radio.openReadingPipe(1,addresses[1]);     // Read on pipe 1, as address 1
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

       role = role_pong_back;                    // Become the primary receiver (pong back)
       radio.openWritingPipe(addresses[1]);      // Since only two radios involved, both listen on the same addresses and pipe numbers in RX mode
       radio.openReadingPipe(1,addresses[0]);    // then switch pipes & addresses to transmit.
       radio.startListening();                   // Need to start listening after opening new reading pipes
    }
  }
}

/*
   March 2014 - TMRh20 - Updated along with High Speed RF24 Library fork
   Parts derived from examples by J. Coliz <maniacbug@ymail.com>
*/
/**
 * Example for efficient call-response using ack-payloads
 *
 * This example continues to make use of all the normal functionality of the radios including
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well.
 * This allows very fast call-response communication, with the responding radio never having to
 * switch out of Primary Receiver mode to send back a payload, but having the option to switch to
 * primary transmitter if wanting to initiate communication instead of respond to a commmunication.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);

byte counter = 1;                                                          // A single byte to keep track of the data being sent back and forth

const uint64_t pipe = 0xE8E8F0F0E1LL;
void setup(){

  Serial.begin(9600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // Setup and configure radio

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipe);        // Both radios listen on the same pipes by default, and switch when writing

  radio.startListening();                 // Start listening
  radio.powerUp();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void) {


/****************** Ping Out Role ***************************/
/********************** SENDER *******************************/

    byte gotByte;                                           // Initialize a variable for the incoming response

    radio.stopListening();                                  // First, stop listening so we can talk.
    printf("Now sending %d as payload. ",counter);          // Use a simple byte counter as payload
    unsigned long time = micros();                          // Record the current microsecond count

    if ( radio.write(&counter, 1) ){                         // Send the counter variable to the other radio
        if(!radio.available()){                             // If nothing in the buffer, we got an ack but it is blank
            printf("Got blank response. round-trip delay: %lu microseconds\n\r",micros()-time);
        }else{
            while(radio.available() ){                      // If an ack with payload was received
                radio.read( &gotByte, 1 );                  // Read it, and display the response time
                printf("Got response %d, round-trip delay: %lu microseconds\n\r",gotByte,micros()-time);
                counter++;                                  // Increment the counter variable
            }
        }

    }else{        printf("Sending failed.\n\r"); }          // If no ack response, sending failed

    delay(1000);  // Try again later
  }

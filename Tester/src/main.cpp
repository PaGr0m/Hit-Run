// #include <IRremote.h>
//
// int RECV_PIN = 12;
//
// IRrecv irrecv(RECV_PIN);
//
// decode_results results;
//
// void setup()
// {
//   Serial.begin(9600);
//   // In case the interrupt driver crashes on setup, give a clue
//   // to the user what's going on.
//   Serial.println("Enabling IRin");
//   irrecv.enableIRIn(); // Start the receiver
//   Serial.println("Enabled IRin");
// }
//
// void loop() {
//   if (irrecv.decode(&results)) {
//     Serial.println(results.value, HEX);
//
//     switch (results.value) {
//       case 0xFF42BD:
//       {
//
//       }
//
//     }
//     irrecv.resume(); // Receive the next value
//   }
//   delay(100);
// }

#include <Arduino.h>

#define PIN_LED_1 5
#define PIN_LED_2 6
#define PIN_LED_3 9
#define PIN_LED_4 10
#define PIN_LED_5 11


void setup() {
  pinMode(PIN_LED_2, OUTPUT);
}

void loop() {
  for (int i = 0; i <= 255; i++)
  {
    analogWrite(PIN_LED_1, i);
    analogWrite(PIN_LED_2, i);
    analogWrite(PIN_LED_3, i);
    analogWrite(PIN_LED_4, i);
    analogWrite(PIN_LED_5, i);
    delay(5);
  }

  for (int i = 255; i >= 0; i--)
  {
    analogWrite(PIN_LED_1, i);
    analogWrite(PIN_LED_2, i);
    analogWrite(PIN_LED_3, i);
    analogWrite(PIN_LED_4, i);
    analogWrite(PIN_LED_5, i);
    delay(5);
  }
  delay(350);
}

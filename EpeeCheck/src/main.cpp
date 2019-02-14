#include <Arduino.h>
#include <SPI.h>
#include "printf.h"

#define pinButton 4


int buttonState = 0;
byte counter = 0;

void setup()
{
    pinMode(pinButton, INPUT_PULLUP);

    Serial. begin(9600);
    printf_begin();
    printf("Hello World\n");
}

void loop()
{
  buttonState = !digitalRead(pinButton);

  counter++;

  if (buttonState)
  {
    printf("Var = %d\n", counter);
  }
}

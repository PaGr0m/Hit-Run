#include <Arduino.h>

#include <SPI.h>

#include <LiquidCrystal.h>
#include "printf.h"

#define PIN_LCD_RS  7
#define PIN_LCD_E   6
#define PIN_LCD_DB4 5
#define PIN_LCD_DB5 4
#define PIN_LCD_DB6 3
#define PIN_LCD_DB7 2

// Initialize
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4,
                  PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);

int data;


void setup()
{
    Serial.begin(9600);
    printf_begin();
    printf("\n\r ***** CONTROLLER ****** \n\r");

    // Wait for console opening
    delay(3000);
}

void loop()
{
    if (Serial.available())
    {
        data = Serial.read();

        lcd.setCursor(0, 1);
        // lcd.write(data);
        lcd.print(char(data));
    }
}

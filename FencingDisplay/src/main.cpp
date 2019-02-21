#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // (RS, E, DB4, DB5, DB6, DB7)

byte first = 1;
byte second = 2;
byte column = 5;

signed short minutes;
signed short secondes;
char timeline[5];

void setup(){
  lcd.begin(16, 2);                  // Задаем размерность экрана

  // lcd.setCursor(0, 0);              // Устанавливаем курсор в начало 1 строки
  // lcd.print("Den, bratan");       // Выводим текст
  // lcd.setCursor(0, 1);              // Устанавливаем курсор в начало 2 строки
  // lcd.print("Dobriy vecher!");         // Выводим текст
  lcd.setCursor(0, 0);
  lcd.print("RED   :   GREEN");

  lcd.setCursor(0, 1);
  lcd.print("Round 1");
}



void loop()
{
  if (first < 10)
    column = 5;
  else
    column = 4;

  lcd.setCursor(column, 0);
  lcd.print(first);

  lcd.setCursor(7, 0);
  lcd.print(second);

  // // CUSTON TIME : BEGIN
  // lcd.setCursor(10, 1);
  // sprintf(timeline,"%d:%d", minutes, secondes);
  // lcd.print(timeline);
  //
  // delay(1000);
  // secondes++;
  //
  // if (secondes == 60)
  // {
  //   secondes = 0;
  //   minutes ++;
  // }
  // // CUSTOM TIME : END
}

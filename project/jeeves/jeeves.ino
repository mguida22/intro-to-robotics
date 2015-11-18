#include <Sparki.h>

void setup()
{
  sparki.clearLCD();
  sparki.print("Welcome to Jeeves!");
  sparki.updateLCD();

  Serial1.begin(9600);
}

void loop()
{
  if (Serial1.available())
  {
    int inByte = Serial1.read();
    sparki.print((char) inByte);
    sparki.updateLCD();
  }
}

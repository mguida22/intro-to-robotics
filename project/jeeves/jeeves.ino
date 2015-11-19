#include <Sparki.h>

void retrieve()
{
  sparki.println("Retrieve");
  sparki.updateLCD();
}

void park()
{
  sparki.println("Park");
  sparki.updateLCD();
}

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
    
    if (inByte == "0")
    {
      retrieve();
    }
    
    if (inByte == "1")
    {
      park();
    }
  }
}

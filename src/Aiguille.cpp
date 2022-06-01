#include <Arduino.h>
#include "Aiguille.h"

// Constructor
Aiguille::Aiguille() : id(0),
                       servoPin(255),
                       railPinAig(255),
                       togPinAig(255),
                       ledPinAig(255),
                       relPinAig(255),
                       minPosition(1500),
                       maxPosition(1500),
                       posAig(0),
                       dirAig(0),
                       lastMoveTime(0),
                       tempo(5)
{
}

void Aiguille::setup(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint16_t g, uint16_t h, uint8_t i = 0)
{
  id = a;
  servoPin = b;
  railPinAig = c;
  togPinAig = d;
  ledPinAig = e;
  relPinAig = f;
  minPosition = g;
  maxPosition = h;
  dirAig = i;
  
  pinMode(ledPinAig, OUTPUT);
  pinMode(relPinAig, OUTPUT);
  pinMode(togPinAig, INPUT_PULLUP);
  if(railPinAig < NOPIN)
    pinMode(railPinAig, INPUT);
}

void Aiguille::test()
{
  this->attach(servoPin);
  this->writeMicroseconds(minPosition);
  delay(500);
  this->writeMicroseconds(maxPosition);
  delay(100);
  posAig = maxPosition;
  this->detach();
}

void Aiguille::loop()
{
  if (posAig != minPosition && posAig != maxPosition)
    this->attach(servoPin);
  else
    this->detach();

  this->togValAig = digitalRead(togPinAig);   // read the input togPinAig
  this->railValAig = digitalRead(railPinAig); // read the input railPinAig

  if (millis() - lastMoveTime > tempo)
  {
    this->lastMoveTime = millis();
    if (togValAig == LOW)
    {                                // check status toggle switche Turnout
      digitalWrite(relPinAig, HIGH); // turn relay turnout on 
      if (dirAig == 0)
      { //  check status of dir variable turnout
        if (posAig > minPosition)
        // if (railValAig == LOW && posAig > minPosition)
          posAig--;
        else
          dirAig = 1; //  set status of dir variable turnout to 1 
      }
      else if (dirAig == 1)
      {
        digitalWrite(ledPinAig, HIGH); // turn LED turnout "straight" on 
        digitalWrite(relPinAig, LOW);  // turn  relay turnout off  
      }
    }
    else if (togValAig == HIGH)
    {                                // check status of toggle switche turnout
      digitalWrite(relPinAig, HIGH); // turn relay turnout on 
      if (dirAig == 1)
      {                           //  check status of dir variable turnout
        if (posAig < maxPosition) //  check status of rail feedback turnout
        // if (railValAig == LOW && posAig < maxPosition)
          posAig++;
        else 
          dirAig = 0; //  set status of dir variable turnout to 0
      }
      else if (dirAig == 0)
      {
        digitalWrite(ledPinAig, LOW); // turn LED turnout "divert" on 
        digitalWrite(relPinAig, LOW); // turn relay turnout off  
      }
    }
    this->writeMicroseconds(posAig); // move servo turnout
  }
}

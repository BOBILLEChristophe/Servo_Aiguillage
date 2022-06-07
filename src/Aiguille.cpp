#include <Arduino.h>
#include "Aiguille.h"

// Constructor
Aiguille::Aiguille() : id(0),
                       servoPin(255),
                       togPinAig(255),
                       ledPinAig(255),
                       minPosition(1500),
                       maxPosition(1500),
                       posAig(0),
                       dirAig(0),
                       lastMoveTime(0),
                       tempo(5)
{}

void Aiguille::setup(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t e, uint16_t f, uint8_t g = 0)
{
  id = a;
  servoPin = b;
  togPinAig = c;
  ledPinAig = d;
  minPosition = e;
  maxPosition = f;
  dirAig = g;

  pinMode(ledPinAig, OUTPUT);
  pinMode(togPinAig, INPUT);
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

void Aiguille::reglageServo(uint8_t x)
{
  this->attach(servoPin);
  if (x == '-')
    this->posAig--;
  else if (x == '+')
    this->posAig++;
  delay(10);
  this->writeMicroseconds(posAig);
  delay(10);
  this->detach();
  Serial.println(posAig);
}

void Aiguille::saveMinPos()
{
  this->minPosition = posAig;
  EEPROM.put((id * 2) * sizeof(uint16_t), posAig);
}

void Aiguille::saveMaxPos()
{
  this->maxPosition = posAig;
  EEPROM.put(((id * 2) + 1) * sizeof(uint16_t), posAig);
}

void Aiguille::writePosServo()
{
  Serial.print("\t");
  Serial.print(this->id);
  Serial.print("\t");
  Serial.print(this->minPosition);
  Serial.print("\t");
  Serial.println(this->maxPosition);
}

void Aiguille::loop()
{
  if (posAig != minPosition && posAig != maxPosition)
    this->attach(servoPin);
  else
    this->detach();

  this->togValAig = digitalRead(togPinAig); // read the input togPinAig

  if (millis() - lastMoveTime > tempo)
  {
    this->lastMoveTime = millis();
    if (togValAig == LOW)
    { // check status toggle switche Turnout

      if (dirAig == 0) //  check status of dir variable turnout
      {
        if (posAig > minPosition) // check status of posAig
          posAig--;
        else
          dirAig = 1; //  set status of dir variable turnout to 1
      }
      else if (dirAig == 1)
      {
        digitalWrite(ledPinAig, HIGH); // turn LED turnout "straight" on
      }
    }
    else if (togValAig == HIGH)
    { // check status of toggle switche turnout
      if (dirAig == 1)
      {                           //  check status of dir variable turnout
        if (posAig < maxPosition) // check status of posAig
          posAig++;
        else
          dirAig = 0; //  set status of dir variable turnout to 0
      }
      else if (dirAig == 0)
      {
        digitalWrite(ledPinAig, LOW); // turn LED turnout "divert" on
      }
    }
    this->writeMicroseconds(posAig); // move servo turnout
  }
}

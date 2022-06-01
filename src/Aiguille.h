/*


*/

#ifndef __AIGUILLE_H__
#define __AIGUILLE_H__

#include <Arduino.h>
#include <Servo.h>

class Aiguille : public Servo
{

private:
  uint8_t  id;
  uint8_t  servoPin;
  uint8_t  railPinAig; // pins allocated to rail volt status for turnout
  uint8_t  togPinAig;  // choose the input pin of toggle switches for simple turnout
  uint8_t  ledPinAig;  // pins allocated to Led control for simple turnout
  uint8_t  relPinAig;
  uint16_t minPosition;
  uint16_t maxPosition;
  uint16_t posAig;
  uint8_t  dirAig;
  uint8_t  togValAig;
  uint8_t  railValAig; // fin de course
  unsigned long lastMoveTime;
  const unsigned long tempo;

public:
  Aiguille(); // Constructor
  void setup(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t, uint8_t);
  void loop();
  void test();
};

#endif
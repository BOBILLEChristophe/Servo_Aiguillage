#include <Arduino.h>
#include <Servo.h>

class Aiguille : public Servo
{

private:
  uint8_t id;
  uint8_t servoPin;
  uint8_t railPinAig; // pins allocated to rail volt status for turnout
  uint8_t togPinAig;  // choose the input pin of toggle switches for simple turnout
  uint8_t ledPinAig;  // pins allocated to Led control for simple turnout
  uint8_t relPinAig;
  uint16_t minPosition;
  uint16_t maxPosition;
  uint8_t posAig;
  uint8_t dirAig;
  uint8_t togValAig;
  uint8_t railValAig;
  uint64_t lastMoveTime;
  uint64_t tempo;

public:
  Aiguille(); // Constructor
  void setup(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t, uint8_t);
  void loop();
  void test();
};

// Constructor
Aiguille::Aiguille() : id(0),
                       servoPin(0),
                       railPinAig(0),
                       togPinAig(0),
                       ledPinAig(0),
                       relPinAig(0),
                       minPosition(0),
                       maxPosition(0),
                       posAig(0),
                       dirAig(0),
                       tempo(10)
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
  lastMoveTime = millis();
  pinMode(ledPinAig, OUTPUT);
  pinMode(relPinAig, OUTPUT);
  pinMode(togPinAig, INPUT);
  pinMode(railPinAig, INPUT);
}

void Aiguille::test()
{
  this->attach(servoPin);
  for (byte i = 0; i < 3; i++)
  {
    this->writeMicroseconds(minPosition);
    delay(1000);
    this->writeMicroseconds(maxPosition);
    delay(1000);
  }
  this->detach();
}


void Aiguille::loop()
{
  this->togValAig = digitalRead(togPinAig);   // read the input togPinAig
  this->railValAig = digitalRead(railPinAig); // read the input railPinAig
  if (millis() - lastMoveTime > tempo)
  {
    this->attach(servoPin);
    if (togValAig == LOW)
    {                                // check status toggle switche Turnout//
      digitalWrite(relPinAig, HIGH); // turn relay turnout on //
      if (dirAig == 1)
      { //  check status of dir variable turnout5//
        if (railValAig == LOW && posAig > minPosition)
          posAig--;
        else
          dirAig = 0; //  set status of dir variable turnout to 0 //
      }
      else
      {
        digitalWrite(ledPinAig, HIGH); // turn LED turnout "straight" on //
        digitalWrite(relPinAig, LOW);  // turn  relay turnout off  //
      }
    }
    else if (togValAig == HIGH)
    {                                // check status of toggle switche turnout5//
      digitalWrite(relPinAig, HIGH); // turn relay turnout on //
      if (dirAig == 0)
      {                                                //  check status of dir variable turnout5//
        if (railValAig == LOW && posAig < maxPosition) //  check status of rail feedback turnout5//
          posAig++;
        else
          dirAig = 1; //  set status of dir variable turnout to 1 //
      }
      else
      {
        digitalWrite(ledPinAig, LOW); // turn LED turnout "divert" on //
        digitalWrite(relPinAig, LOW); // turn relay turnout off  //
      }
    }
    this->writeMicroseconds(posAig); // move servo turnout
    this->detach();
    this->lastMoveTime = millis();
  }
}

#define NB_SERVO 2
Aiguille aiguille[NB_SERVO]; // Tableau contenant 'NB_SERVO' instances (objets)

void setup()
{
  // Serial.begin(9600); // start the serial port
  Serial.begin(115200); // start the serial port

  // Setup de chaque instance
  // id, servoPin, railPinAig, togPinAig, ledPinAig, relPinAig, minPosition, maxPosition, (dirAig)
  aiguille[0].setup(5, 6, 30, 39, 48, 65, 750, 2200, 0);
  aiguille[1].setup(6, 7, 32, 41, 50, 66, 1250, 1750, 0);
  
   for (auto el : aiguille)
    el.test();
}

void loop()
{
  for (auto el : aiguille)
    el.loop();
}

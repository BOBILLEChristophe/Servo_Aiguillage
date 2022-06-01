#include <Arduino.h>
#include "Aiguille.h"

#define NOPIN 255
#define NB_SERVO 6
Aiguille aiguille[NB_SERVO]; // Tableau contenant 'NB_SERVO' instances (objets)

void setup()
{
  // Serial.begin(9600); // start the serial port
  Serial.begin(115200); // start the serial port

  // Setup de chaque instance
  // id, servoPin, railPinAig, togPinAig, ledPinAig, relPinAig, minPosition, maxPosition, (dirAig)
  aiguille[0].setup(1, 2, 49, 31, 40, 65, 750, 2200, 0);
  aiguille[1].setup(2, 3, 51, 33, 42, 66, 1250, 1750, 0);
  aiguille[2].setup(3, 4, 53, 35, 44, 65, 1400, 1800, 0);
  aiguille[3].setup(4, 5, NOPIN, 37, 46, 66, 1250, 1750, 0);
  aiguille[4].setup(5, 6, NOPIN, 39, 48, 65, 1000, 2000, 0);
  aiguille[5].setup(6, 11, NOPIN, 41, 50, 66, 1300, 2200, 0);

  for (byte i = 0; i < NB_SERVO; i++)
    aiguille[i].test();
}

void loop()
{
  for (byte i = 0; i < NB_SERVO; i++)
    aiguille[i].loop();
}

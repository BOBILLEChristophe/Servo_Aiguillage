#include <Arduino.h>
#include <EEPROM.h>
#include "Aiguille.h"

#define NB_SERVO 6
Aiguille aiguille[NB_SERVO]; // Tableau contenant 'NB_SERVO' instances (objets)

void reglageServo(); // Definition de la fonction
char incomingByte;
uint8_t numAig = 0;
uint8_t etapeNum = 0;
uint16_t position[NB_SERVO * 2];

// EEPROM
uint8_t eepromAddress[NB_SERVO];

void setup()
{
  // Serial.begin(9600); // start the serial port
  Serial.begin(115200); // start the serial port

  for (byte i = 0; i < NB_SERVO * 2; i++)
  {
    uint16_t val;
    EEPROM.get(i * sizeof(uint16_t), val);
    if (val == 0)
      position[i] = 1500;
    else
      position[i] = val;
  }

  // Temporaire
  position[0] = 750;
  position[1] = 2200;
  position[2] = 1250;
  position[3] = 1750;
  position[4] = 1400;
  position[5] = 1800;
  position[6] = 1250;
  position[7] = 1750;
  position[8] = 1000;
  position[9] = 2000;
  position[10] = 1300;
  position[11] = 2200;

  // Setup de chaque instance
  // id, servoPin, railPinAig, togPinAig, ledPinAig, relPinAig, minPosition, maxPosition, (dirAig)
  aiguille[0].setup(0, 2, 49, 31, 40, 65, position[0], position[1], 0);
  aiguille[1].setup(1, 3, 51, 33, 42, 66, position[2], position[3], 0);
  aiguille[2].setup(2, 4, 53, 35, 44, 65, position[4], position[5], 0);
  aiguille[3].setup(3, 5, NOPIN, 37, 46, 66, position[6], position[7], 0);
  aiguille[4].setup(4, 6, NOPIN, 39, 48, 65, position[8], position[9], 1);
  aiguille[5].setup(5, 11, NOPIN, 41, 50, 66, position[10], position[11], 0);

  for (byte i = 0; i < NB_SERVO; i++)
    aiguille[i].test();
}

void loop()
{
  for (byte i = 0; i < NB_SERVO; i++)
    aiguille[i].loop();

  reglageServo();
}

void reglageServo()
{
  if (Serial.available())
  {
    incomingByte = Serial.read();
    Serial.println(incomingByte);
    if (incomingByte > 47 && incomingByte < 58)
    {
      numAig = (uint8_t)incomingByte - 48;
      Serial.print("Réglage de l'aiguille ");
      Serial.println(numAig);
    }
    else
    {
      switch (incomingByte)
      {
      case '-':
        Serial.print("position mini : ");
        aiguille[numAig].reglageServo('-');
        etapeNum = 0;
        break;
      case '+':
        Serial.print("position max : ");
        aiguille[numAig].reglageServo('+');
        etapeNum = 1;
        break;
      case 's':
        switch (etapeNum)
        {
        case 0:
          aiguille[numAig].saveMinPos();
          break;
        case 1:
          aiguille[numAig].saveMaxPos();
          break;
        }
        etapeNum++;
        break;
        case 'w':
          Serial.print("Aiguille ");
          Serial.print("\t");
          Serial.print("Min");
          Serial.print("\t");
          Serial.println("Max");
          for (byte i = 0; i < NB_SERVO; i++)
            aiguille[i].writePosServo();
          break;
        default:
          Serial.print("Erreur de saisie");
      }
    }
  }
}



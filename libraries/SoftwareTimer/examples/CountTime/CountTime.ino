/*
  Created by Bruno Calou Alves, May, 2015.
  Link - https://github.com/brunocalou/Timer
  Read LICENSE for more information.
*/

#include "SoftwareTimer.h"

//Timer object
SoftwareTimer softwareTimer;

void setup()
{
  Serial.begin(9600);

  //Start the softwareTimer
  softwareTimer.start();
}

void loop()
{
  //Print the elapsed time
  Serial.println(softwareTimer.getElapsedTime());

  //Update the softwareTimer
  softwareTimer.update();
}

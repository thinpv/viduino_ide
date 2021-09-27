/*
  Created by Bruno Calou Alves, May, 2015.
  Link - https://github.com/brunocalou/Timer
  Read LICENSE for more information.
*/

#include "SoftwareTimer.h"

//Timer object
SoftwareTimer softwareTimer;

int counter = 5;

void countdown() {
  Serial.println(counter);
  if(counter > 0) {
    counter -= 1;
  }

}
void setup()
{
  Serial.begin(9600);

  //Set the interval in milliseconds
  //and the number of repetitions
  softwareTimer.setInterval(1000, 6);

  //Set our callback function
  softwareTimer.setCallback(countdown);

  //Start the softwareTimer
  softwareTimer.start();
}

void loop()
{
  //Update the softwareTimer
  softwareTimer.update();
}

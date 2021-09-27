/*
  Created by Bruno Calou Alves, May, 2015.
  Link - https://github.com/brunocalou/Timer
  Read LICENSE for more information.
*/

#include "SoftwareTimer.h"

int led_pin = 13;  //Set our led pin

//Timer object
SoftwareTimer softwareTimer;

//Set our awesome turn on led function
void turnOnLed() {
  digitalWrite(led_pin, HIGH);
}

void setup()
{
  pinMode(led_pin, OUTPUT);

  //The led will turn on after 3 seconds
  softwareTimer.setTimeout(3000);

  //Set our callback function
  softwareTimer.setCallback(turnOnLed);

  //Start the softwareTimer
  softwareTimer.start();
}

void loop()
{
  //Update the softwareTimer
  softwareTimer.update();
}

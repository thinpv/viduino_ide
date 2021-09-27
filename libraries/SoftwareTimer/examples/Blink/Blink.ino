/*
  Created by Bruno Calou Alves, May, 2015.
  Read LICENSE for more information.
*/

#include "SoftwareTimer.h"

int led_pin = 13;  //Set our led pin
bool led_is_on = false;  //Holds if the led is on

//Timer object
SoftwareTimer softwareTimer;

//Set our awesome blink led function
void blinkLed() {
  if(led_is_on) {
    digitalWrite(led_pin, HIGH);
  } else {
    digitalWrite(led_pin, LOW);
  }

  //Change our led state
  led_is_on = !led_is_on;
}

void setup()
{
  pinMode(led_pin, OUTPUT);

  //Set the interval in milliseconds we want the led to blink
  softwareTimer.setInterval(1000);

  //Set our callback function
  softwareTimer.setCallback(blinkLed);

  //Start the timer
  softwareTimer.start();
}

void loop()
{
  //Update the timer
  softwareTimer.update();
}

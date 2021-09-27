/*
  Created by Bruno Calou Alves, May, 2015.
  Link - https://github.com/brunocalou/Timer
  Read LICENSE for more information.
*/

#include "SoftwareTimer.h"

//Timer object
SoftwareTimer softwareTimer;

//incomming command
char command = ' ';

void printElapsedTime() {
  Serial.print("Elapsed time = ");
  Serial.print(softwareTimer.getElapsedTime());
  Serial.println(" ms");
}

void setup()
{
  Serial.begin(9600);

  //Set the interval in milliseconds we want the led to blink
  softwareTimer.setInterval(1000);

  //Set our callback function
  softwareTimer.setCallback(printElapsedTime);

  //Start the timer
  softwareTimer.start();
}

void loop()
{
  if(Serial.available()) {
    command = Serial.read();

    if(command == 's') {
      softwareTimer.start();
      Serial.println("Started");
    } else if(command == 't') {
      softwareTimer.stop();
      Serial.println("Stopped");
    } else if(command == 'p') {
      softwareTimer.pause();
      Serial.println("Paused");
    } else if(command == 'r') {
      softwareTimer.reset();
      Serial.println("Resetted");
    }
  }
  //Update the timer
  softwareTimer.update();
}

/*
  Created by Bruno Calou Alves, May, 2015.
  Link - https://github.com/brunocalou/Timer
  Read LICENSE for more information.
*/

#include "SoftwareTimer.h"

//Create our class (FooLed, got it ?)
class FooLed {
public:
  FooLed(byte led_pin) {
    this->led_pin = led_pin;
    pinMode(led_pin, OUTPUT);
    led_is_on = false;
  }

  //Our awesome blink method
  void blink() {
    if(led_is_on) {
        digitalWrite(led_pin, HIGH);
      } else {
        digitalWrite(led_pin, LOW);
      }

      //Change our led state
      led_is_on = !led_is_on;
  }

private:
  byte led_pin;
  byte led_is_on;
};


//FooLed object
FooLed led(13);

//TimerForMethods object
TimerForMethods<FooLed> softwareTimer(&led, &FooLed::blink);

void setup()
{
  //Set the interval in milliseconds we want the led to blink
  softwareTimer.setInterval(1000);

  //Start the softwareTimer
  softwareTimer.start();
}

void loop()
{
  //Update the softwareTimer
  softwareTimer.update();
}

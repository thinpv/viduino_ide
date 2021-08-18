#include <Wire.h>
#include "Goodix.h"

// D5 D6 pins are defined on boards like WeMOS, You can redefine it to numeric values
#define INT_PIN 6
#define RST_PIN 13

Goodix touch = Goodix();
char buffer[200];

void handleTouch(int8_t contacts, GTPoint *points) {
  if(contacts > 0) {
  Serial.print("Contacts: ");
  Serial.println(contacts);
  for (uint8_t i = 0; i < contacts; i++) {
    sprintf(buffer, "C%d: #%d %d,%d s:%d\n", i, points[i].trackId, points[i].x, points[i].y, points[i].area);
    Serial.print (buffer);
    yield();
  }
  }
}

void touchStart() {
 if (touch.begin(INT_PIN, RST_PIN)!=true) {
    Serial.println("! Module reset failed");
  } else {
    Serial.println("Module reset OK");
  }
  
  Serial.print("Check ACK on addr request on 0x");
  Serial.println(touch.i2cAddr, HEX);
  
  Wire.beginTransmission(touch.i2cAddr);  
  int error = Wire.endTransmission();
  if (error == 0) {    
    Serial.println(": SUCCESS");   
  } else {
    Serial.print(": ERROR #");
    Serial.println(error);
  }
  touch.fwResolution(800, 480);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nGoodix GT911x touch driver");

  Wire.setClock(400000);
  Wire.begin();

  touch.setHandler(handleTouch);
  touchStart();
}

void loop() {
  touch.loop();
  delay(1);
}
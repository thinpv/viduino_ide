#include "tusb.h"

void setup() {
  // put your setup code here, to run once:
  tusb_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  tud_task();
}

#include "touchscreen.h"

TouchScreen touchscreen;

int touchscreen_status = 0;
int touchscreen_x = 0;
int touchscreen_y = 0;
int status_last = TOUCH_NONE;
int x_last = 0;
int y_last = 0;
bool draw = false;


void setup()
{
    Serial.begin(115200);
    touchscreen.begin();
    touchscreen.calibrate(800, 480);
}

void loop()
{
    touchscreen.read();
    touchscreen_status = touchscreen.getStatus();
    touchscreen_x = touchscreen.getX();
    touchscreen_y = touchscreen.getY();
    Serial.print("X: ");
    Serial.print(touchscreen_x);
    Serial.print(", Y: ");
    Serial.print(touchscreen_y);
    Serial.print(", S: ");
    Serial.println(touchscreen_status);
}

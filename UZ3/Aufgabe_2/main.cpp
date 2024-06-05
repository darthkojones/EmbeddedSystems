#include <Arduino.h>
#include "ampel.h"

#define RGB_LED_PIN A4
#define MOTION_SENSOR_PIN 5
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define LIGHT_SENSOR_PIN A2

Ampel ampel(RGB_LED_PIN, MOTION_SENSOR_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN, LIGHT_SENSOR_PIN);

void setup() {
    Serial.begin(9600);
    ampel.begin();
}

void loop() {
    ampel.update();
    delay(100);
}

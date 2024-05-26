#include "dibse.h"
#include <Arduino.h>

DibsE::DibsE(int pin, int numPixels) 
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ800), blinkDuration(0), red(0), green(0), blue(0), blinking(false), lastBlinkTime(0), ledState(false) {
}

void DibsE::setup() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void DibsE::loop() {
    if (blinking) {
        unsigned long currentTime = millis();
        if (currentTime - lastBlinkTime >= blinkDuration) {
            ledState = !ledState;
            if (ledState) {
                strip.fill(strip.Color(red, green, blue), 0, strip.numPixels());
            } else {
                strip.clear();
            }
            strip.show();
            lastBlinkTime = currentTime;
        }
    }
}

void DibsE::simpleBlinkOn(int duration, int red, int green, int blue) {
    this->blinkDuration = duration;
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->blinking = true;
}

void DibsE::simpleBlinkOff() {
    this->blinking = false;
    strip.clear();
    strip.show();
}

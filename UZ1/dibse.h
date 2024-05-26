#ifndef DIBSE_H
#define DIBSE_H

#include <Adafruit_NeoPixel.h>

class DibsE {
public:
    DibsE(int pin, int numPixels);

    void setup();
    void loop();
    void simpleBlinkOn(int duration, int red, int green, int blue);
    void simpleBlinkOff();

private:
    Adafruit_NeoPixel strip;
    int blinkDuration;
    int red;
    int green;
    int blue;
    bool blinking;
    unsigned long lastBlinkTime;
    bool ledState;
};

#endif // DIBSE_H

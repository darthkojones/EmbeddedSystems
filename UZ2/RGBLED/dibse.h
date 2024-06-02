#ifndef DIBSE_H
#define DIBSE_H

#include <Arduino.h>

class DibsE {
  private:
    int redPin;
    int greenPin;
    int bluePin;
    unsigned long previousMillis;
    int blinkInterval;
    bool ledState;

  public:
    DibsE(int rPin, int gPin, int bPin);

    void setup();

    void setColor(int red, int green, int blue);

    void simpleBlinkOn(int interval, int red, int green, int blue);

    void loop();
};

#endif // DIBSE_H

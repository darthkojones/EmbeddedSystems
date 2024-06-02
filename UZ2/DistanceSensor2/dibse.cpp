#include "dibse.h"

DibsE::DibsE(int rPin, int gPin, int bPin) : redPin(rPin), greenPin(gPin), bluePin(bPin), previousMillis(0), blinkInterval(500), ledState(false) {}

void DibsE::setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void DibsE::setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void DibsE::simpleBlinkOn(int interval, int red, int green, int blue) {
  blinkInterval = interval;
  setColor(red, green, blue);
}

void DibsE::loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    if (ledState) {
      setColor(255, 255, 255);
    } else {
      setColor(0, 0, 0);
    }
  }
}

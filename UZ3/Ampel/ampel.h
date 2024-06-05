#ifndef AMPEL_H
#define AMPEL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Ampel {
public:
    Ampel(int rgbPin, int motionPin, int joystickPin, int lightSensorPin);

    void begin();
    void update();
    void setDayMode(bool isDay);

private:
    int rgbPin;
    int motionPin;
    int joystickPin;
    int lightSensorPin;
    Adafruit_NeoPixel rgbLed;

    unsigned long previousMillis;
    bool isDayMode;
    bool pedestrianDetected;
    bool emergencyVehicleDetected;

    enum State { RED, RED_TO_YELLOW, GREEN, GREEN_BLINKING, YELLOW, OFF };
    State currentState;

    void setColor(int red, int green, int blue);
    void checkSensors();
    void changeState(State newState, unsigned long duration);
    void handleState();
};

#endif // AMPEL_H

#include "ampel.h"

Ampel::Ampel(int rgbPin, int motionPin, int joystickPin, int lightSensorPin)
    : rgbPin(rgbPin), motionPin(motionPin), joystickPin(joystickPin), lightSensorPin(lightSensorPin),
      rgbLed(1, rgbPin, NEO_GRB + NEO_KHZ800), previousMillis(0), isDayMode(true),
      pedestrianDetected(false), emergencyVehicleDetected(false), currentState(RED) {}

void Ampel::begin() {
    rgbLed.begin();
    pinMode(motionPin, INPUT);
    pinMode(joystickPin, INPUT);
    pinMode(lightSensorPin, INPUT);
    changeState(RED, 7000);
}

void Ampel::update() {
    checkSensors();
    handleState();
}

void Ampel::setDayMode(bool isDay) {
    isDayMode = isDay;
    if (!isDayMode) {
        changeState(OFF, 0);
    } else {
        changeState(RED, 7000);
    }
}

void Ampel::setColor(int red, int green, int blue) {
    rgbLed.setPixelColor(0, rgbLed.Color(red, green, blue));
    rgbLed.show();
}

void Ampel::checkSensors() {
    if (digitalRead(motionPin) == HIGH) {
        pedestrianDetected = true;
    }

    if (digitalRead(joystickPin) == HIGH) {
        emergencyVehicleDetected = true;
    }

    int lightLevel = analogRead(lightSensorPin);
    if (lightLevel < 512) { 
        setDayMode(false);
    } else {
        setDayMode(true);
    }
}

void Ampel::changeState(State newState, unsigned long duration) {
    currentState = newState;
    previousMillis = millis();
    if (duration > 0) {
        previousMillis += duration;
    }
}

void Ampel::handleState() {
    unsigned long currentMillis = millis();

    if (isDayMode) {
        switch (currentState) {
            case RED:
                setColor(255, 0, 0);
                if (currentMillis - previousMillis >= 7000) {
                    changeState(RED_TO_YELLOW, 3000);
                }
                if (pedestrianDetected) {
                    changeState(YELLOW, 3000);
                    pedestrianDetected = false;
                }
                if (emergencyVehicleDetected) {
                    changeState(RED_TO_YELLOW, 3000);
                    emergencyVehicleDetected = false;
                }
                break;
            case RED_TO_YELLOW:
                setColor(255, 255, 0);
                if (currentMillis - previousMillis >= 3000) {
                    changeState(GREEN, 5000);
                }
                break;
            case GREEN:
                setColor(0, 255, 0);
                if (currentMillis - previousMillis >= 5000) {
                    changeState(GREEN_BLINKING, 0);
                }
                if (pedestrianDetected) {
                    changeState(YELLOW, 3000);
                    pedestrianDetected = false;
                }
                break;
            case GREEN_BLINKING:
                if ((currentMillis / 500) % 2 == 0) {
                    setColor(0, 255, 0);
                } else {
                    setColor(0, 0, 0);
                }
                if (currentMillis - previousMillis >= 2000) {
                    changeState(YELLOW, 3000);
                }
                break;
            case YELLOW:
                setColor(255, 255, 0);
                if (currentMillis - previousMillis >= 3000) {
                    changeState(RED, 7000);
                }
                break;
            case OFF:
                if ((currentMillis / 500) % 2 == 0) {
                    setColor(255, 255, 0);
                } else {
                    setColor(0, 0, 0);
                }
                break;
        }
    } else {
        setColor(255, 255, 0);
        if ((currentMillis / 500) % 2 == 0) {
            setColor(255, 255, 0);
        } else {
            setColor(0, 0, 0);
        }
    }
}

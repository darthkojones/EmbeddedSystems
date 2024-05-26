#include "JoystickHigh.h"

JoystickHigh::JoystickHigh(int pinX, int pinY, int pinButton)
    : pinX(pinX), pinY(pinY), pinButton(pinButton), posX(0), posY(0), deadzone(0), buttonPressed(false) {}

void JoystickHigh::begin() {
    pinMode(pinButton, INPUT);
    Serial.begin(9600); // Initialize serial communication for debugging
}

void JoystickHigh::update() {
    posX = readAxis(pinX);
    posY = readAxis(pinY);
    buttonPressed = readButton();
}

int16_t JoystickHigh::getPosX(bool immediate) {
    return immediate ? readAxis(pinX) : posX;
}

int16_t JoystickHigh::getPosY(bool immediate) {
    return immediate ? readAxis(pinY) : posY;
}

bool JoystickHigh::getButton(bool immediate) {
    return immediate ? readButton() : buttonPressed;
}

void JoystickHigh::setDeadzone(int16_t deadzone) {
    this->deadzone = deadzone;
}

int16_t JoystickHigh::getDeadzone() {
    return deadzone;
}

int16_t JoystickHigh::readAxis(int pin) {
    int value = analogRead(pin);
    value = map(value, 0, 1023, -512, 512);
    if (abs(value) < deadzone) {
        value = 0;
    }
    return value;
}

bool JoystickHigh::readButton() {
    return digitalRead(pinButton) == LOW; 
}

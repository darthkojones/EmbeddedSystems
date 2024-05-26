#include "JoystickLow.h"

JoystickLow::JoystickLow(uint8_t pinX, uint8_t pinY, uint8_t pinButton)
    : pinX(pinX), pinY(pinY), pinButton(pinButton), posX(0), posY(0), deadzone(0), buttonPressed(false) {}

void JoystickLow::begin() {
    // Configure button pin as input
    DDRD &= ~(1 << pinButton); 
    // Enable pull-up resistor on button pin
    PORTD |= (1 << pinButton);
}

void JoystickLow::update() {
    posX = readAxis(pinX);
    posY = readAxis(pinY);
    buttonPressed = readButton();
}

int16_t JoystickLow::getPosX(bool immediate) {
    return immediate ? readAxis(pinX) : posX;
}

int16_t JoystickLow::getPosY(bool immediate) {
    return immediate ? readAxis(pinY) : posY;
}

bool JoystickLow::getButton(bool immediate) {
    return immediate ? readButton() : buttonPressed;
}

void JoystickLow::setDeadzone(int16_t deadzone) {
    this->deadzone = deadzone;
}

int16_t JoystickLow::getDeadzone() {
    return deadzone;
}

int16_t JoystickLow::readAxis(uint8_t pin) {
    uint16_t value = readAnalog(pin);
    value = map(value, 0, 1023, -512, 512);
    if (abs(value) < deadzone) {
        value = 0;
    }
    return value;
}

bool JoystickLow::readButton() {
    return (PIND & (1 << pinButton)) == 0; 

uint16_t JoystickLow::readAnalog(uint8_t pin) {
    // Select the appropriate analog input pin
    ADMUX = (ADMUX & 0xF0) | (pin & 0x0F);
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    // Return the result
    return ADC;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

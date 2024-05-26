#ifndef JOYSTICKLOW_H
#define JOYSTICKLOW_H

#include <avr/io.h>
#include <stdint.h>

class JoystickLow {
public:
    JoystickLow(uint8_t pinX, uint8_t pinY, uint8_t pinButton);
    void begin();
    void update();
    int16_t getPosX(bool immediate = false);
    int16_t getPosY(bool immediate = false);
    bool getButton(bool immediate = false);
    void setDeadzone(int16_t deadzone);
    int16_t getDeadzone();

private:
    uint8_t pinX, pinY, pinButton;
    int16_t posX, posY, deadzone;
    bool buttonPressed;

    int16_t readAxis(uint8_t pin);
    bool readButton();
    uint16_t readAnalog(uint8_t pin);
};

#endif // JOYSTICKLOW_H

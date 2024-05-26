#ifndef JOYSTICKHIGH_H
#define JOYSTICKHIGH_H

#include <Arduino.h>

class JoystickHigh {
public:
    JoystickHigh(int pinX, int pinY, int pinButton);
    void begin();
    void update();
    int16_t getPosX(bool immediate = false);
    int16_t getPosY(bool immediate = false);
    bool getButton(bool immediate = false);
    void setDeadzone(int16_t deadzone);
    int16_t getDeadzone();

private:
    int pinX, pinY, pinButton;
    int16_t posX, posY, deadzone;
    bool buttonPressed;

    int16_t readAxis(int pin);
    bool readButton();
};

#endif // JOYSTICKHIGH_H

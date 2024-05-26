#include <iostream>
#include <bitset>
#include "bitoperations.h"
#include "JoystickHigh.h"
#include "JoystickLow.h"
#include "dibse.h"

JoystickHigh joystickHigh(A0, A1, 2); // Example pin numbers for high-level joystick
JoystickLow joystickLow(0, 1, 2); // Example ADC pins and digital pin for low-level joystick
DibsE dibse(6, 8);

void setup() {
    joystickHigh.begin();
    joystickLow.begin();
    Serial.begin(9600);
    dibse.setup();
    dibse.simpleBlinkOn(500, 255, 0, 0);

}

void loop() {
    // Demonstrate the use of bitwise macros
    unsigned int value = 0b00001111; 

    // Set the bit at position 4
    SB(value, 4);
    std::cout << "After setting bit 4: " << std::bitset<8>(value) << std::endl;

    // Clear the bit at position 3
    CB(value, 3);
    std::cout << "After clearing bit 3: " << std::bitset<8>(value) << std::endl;

    // Check if the bit at position 2 is set
    bool isSet = IBS(value, 2);
    std::cout << "Bit 2 is set: " << isSet << std::endl;

    // Toggle the bit at position 1
    TB(value, 1);
    std::cout << "After toggling bit 1: " << std::bitset<8>(value) << std::endl;

    // Update and demonstrate high-level joystick usage
    joystickHigh.update();
    int16_t posXHigh = joystickHigh.getPosX();
    int16_t posYHigh = joystickHigh.getPosY();
    bool buttonHigh = joystickHigh.getButton();

    std::cout << "JoystickHigh - X: " << posXHigh << " Y: " << posYHigh << " Button: " << (buttonHigh ? "Pressed" : "Not Pressed") << std::endl;

    // Update and demonstrate low-level joystick usage
    joystickLow.update();
    int16_t posXLow = joystickLow.getPosX();
    int16_t posYLow = joystickLow.getPosY();
    bool buttonLow = joystickLow.getButton();

    std::cout << "JoystickLow - X: " << posXLow << " Y: " << posYLow << " Button: " << (buttonLow ? "Pressed" : "Not Pressed") << std::endl;
    dibse.loop();
    delay(100); // Update every 100 ms
}

int main() {
    setup();
    while (true) {
        loop();
    }
    return 0;
}
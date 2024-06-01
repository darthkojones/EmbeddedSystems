#include <iostream>
#include <bitset>
#include "dibse.h"

DibsE dibse(6, 8);

void setup() {
    dibse.setup();
    dibse.simpleBlinkOn(500, 255, 0, 0);

}

void loop() {
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
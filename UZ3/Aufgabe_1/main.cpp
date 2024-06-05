#include <Arduino.h>
#include "sleep_delay.h"

void setup() {
    Serial.begin(9600);    
    Serial.println("Starting sleep delay demonstration...");
}

void loop() {
    Serial.println("Going to sleep for 10 seconds...");
    sleep_delay(10); // Sleep for 10 seconds
    Serial.println("Woke up!");

    delay(1000); // Wait for a second before going back to sleep
}

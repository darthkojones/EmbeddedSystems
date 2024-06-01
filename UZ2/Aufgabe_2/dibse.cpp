#include "dibse.h"
#include "LedMatrix.h"

// Define the pins for the RGB LED and the LED Matrix
#define RGB_RED_PIN 3
#define RGB_GREEN_PIN 5
#define RGB_BLUE_PIN 6
#define LATCH_PIN 8
#define DATA_PIN 9
#define CLOCK_PIN 10

// Initialize the LedMatrix object
LedMatrix ledMatrix(LATCH_PIN, DATA_PIN, CLOCK_PIN);

DibsE::DibsE(DistanceSensor sensor) : sensor(sensor) {}

void DibsE::begin() {
    sensor.begin();
    // Initialize RGB LED pins as outputs
    pinMode(RGB_RED_PIN, OUTPUT);
    pinMode(RGB_GREEN_PIN, OUTPUT);
    pinMode(RGB_BLUE_PIN, OUTPUT);
    // Initialize the LED matrix
    ledMatrix.clear();
}

void DibsE::update() {
    int distance = sensor.getDistance();
    
    // Control RGB LED based on distance
    int blinkRate = map(distance, 0, 200, 100, 1000); 
    digitalWrite(RGB_RED_PIN, HIGH);
    delay(blinkRate / 2);
    digitalWrite(RGB_RED_PIN, LOW);
    delay(blinkRate / 2);
    
    // Update LED Matrix
    static byte columnIndex = 0;
    static byte previousColumns[8] = {0};
    
    // Shift previous columns to the right
    for (int i = 7; i > 0; --i) {
        previousColumns[i] = previousColumns[i - 1];
    }
    
    // Map the distance to the LED matrix height (inverted)
    byte newColumn = map(distance, 0, 200, 8, 0); 
    previousColumns[0] = (1 << newColumn) - 1; // Create the mask for the column
    
    // Clear the display buffer
    ledMatrix.clear();
    
    // Draw the updated columns on the LED matrix
    for (int i = 0; i < 8; ++i) {
        ledMatrix.drawColumn(i, previousColumns[i]);
    }
    
    // Update the LED matrix display
    ledMatrix.update();
}

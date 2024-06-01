#include <Arduino.h>
#include "LedMatrix.h"
#include "LedMatrixFont.h"

// Define the pins for the RGB LED and the LED Matrix
#define RGB_RED_PIN 3
#define RGB_GREEN_PIN 5
#define RGB_BLUE_PIN 6
#define LATCH_PIN 11
#define DATA_PIN 16
#define CLOCK_PIN 15

// Define the pins for the ultrasonic sensor
#define TRIG_PIN 8
#define ECHO_PIN 4

// Distance Sensor class
class DistanceSensor {
  private:
    int trigPin;
    int echoPin;
    long duration;
    int distance;

  public:
    DistanceSensor(int tPin, int ePin) {
      trigPin = tPin;
      echoPin = ePin;
    }

    void begin() {
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
    }

    int getDistance() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);

      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;

      // Debugging distance calculation
      Serial.print("Duration: ");
      Serial.println(duration);
      Serial.print("Calculated Distance: ");
      Serial.println(distance);

      return distance;
    }
};

// Initialize LedMatrix object
LedMatrix ledMatrix(LATCH_PIN, DATA_PIN, CLOCK_PIN);
LedMatrixTextBuffer textBuffer(ledMatrix);

// Initialize DistanceSensor object
DistanceSensor sensor(TRIG_PIN, ECHO_PIN);

unsigned long previousDistanceMillis = 0;
unsigned long previousScrollMillis = 0;
const long distanceInterval = 1000;  // Update distance every 1000 milliseconds
const long scrollInterval = 45;     // Update scrolling every 100 milliseconds

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  sensor.begin();
  textBuffer.drawText(" 0 ", 0); // Initial text
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to update the distance
  if (currentMillis - previousDistanceMillis >= distanceInterval) {
    previousDistanceMillis = currentMillis;
    int distance = sensor.getDistance();

    // Create the distance string
    char distanceStr[32];
    snprintf(distanceStr, sizeof(distanceStr), " %d ", distance);

    // Update text buffer with distance string
    textBuffer.drawText(distanceStr, 0);

 
  }

  // Check if it's time to scroll the text
  if (currentMillis - previousScrollMillis >= scrollInterval) {
    previousScrollMillis = currentMillis;

    // Slide text and update LED matrix display
    textBuffer.slideLeft(1);
    textBuffer.update();
  }

  // Efficient update for LED matrix to minimize flickering
  ledMatrix.update();
}

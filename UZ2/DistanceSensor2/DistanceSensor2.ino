#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
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

// Define the pin for the RGB LED
#define RGB_LED_PIN A4

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

// DibsE class for handling RGB LED
class DibsE {
  private:
    int rgbLedPin;
    uint32_t rgbLedColor;
    Adafruit_NeoPixel* rgbLedObject;

  public:
    DibsE(int pin) : rgbLedPin(pin) {
      rgbLedObject = new Adafruit_NeoPixel(1, rgbLedPin, NEO_GRB + NEO_KHZ800);
      rgbLedObject->begin();
    }

    ~DibsE() {
      delete rgbLedObject;
    }

    void setColor(int red, int green, int blue) {
      rgbLedObject->setPixelColor(0, rgbLedObject->Color(red, green, blue));
      rgbLedObject->show();
      Serial.print("Setting color to: R=");
      Serial.print(red);
      Serial.print(", G=");
      Serial.print(green);
      Serial.print(", B=");
      Serial.println(blue);
    }
};

// Function to interpolate color values smoothly
void interpolateColor(int distance, int &red, int &green, int &blue) {
  if (distance <= 20) {
    red = map(distance, 10, 20, 255, 255);
    green = map(distance, 0, 20, 0, 165);
    blue = 0;
  } else if (distance <= 40) {
    red = map(distance, 20, 40, 255, 255);
    green = map(distance, 20, 40, 165, 255);
    blue = 0;
  } else if (distance <= 80) {
    red = map(distance, 40, 80, 255, 0);
    green = map(distance, 40, 80, 255, 255);
    blue = 0;
  } else {
    red = 0;
    green = 255;
    blue = 0;
  }
}

// Initialize LedMatrix object
LedMatrix ledMatrix(LATCH_PIN, DATA_PIN, CLOCK_PIN);
LedMatrixTextBuffer textBuffer(ledMatrix);

// Initialize DistanceSensor and DibsE objects
DistanceSensor sensor(TRIG_PIN, ECHO_PIN);
DibsE myDibse(RGB_LED_PIN);

unsigned long previousDistanceMillis = 0;
unsigned long previousScrollMillis = 0;
const long distanceInterval = 100;  // Update distance every 1000 milliseconds
const long scrollInterval = 45;     // Update scrolling every 45 milliseconds

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

    // Determine the LED color based on the distance with smooth transition
    int red, green, blue;
    interpolateColor(distance, red, green, blue);
    myDibse.setColor(red, green, blue);
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

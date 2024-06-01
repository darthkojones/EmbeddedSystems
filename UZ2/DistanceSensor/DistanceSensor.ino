#include <Arduino.h>

// Define the pins for the LED Matrix
#define LATCH_PIN 11
#define DATA_PIN 16
#define CLOCK_PIN 15

// Define the pins for the ultrasonic sensor
#define TRIG_PIN 8
#define ECHO_PIN 4

// LedMatrix class implementation
class LedMatrix {
  public:
    LedMatrix(int latchPin, int dataPin, int clockPin) {
      this->dataPin = dataPin;
      this->latchPin = latchPin;
      this->clockPin = clockPin;

      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT);

      this->clear();
    }

    void clear() {
      for (int i = 0; i < 8; i++) {
        this->displayBuffer[i] = 0;
      }
    }

    void drawColumn(uint8_t column, byte mask) {
      if (column < 8) {
        displayBuffer[column] = mask;
      }
    }

    void update() {
      digitalWrite(this->latchPin, LOW);
      for (byte i = 0; i < 8; i++) {
        shiftOut(this->dataPin, this->clockPin, MSBFIRST, displayBuffer[i]);
        shiftOut(this->dataPin, this->clockPin, MSBFIRST, 1 << i);
      }
      digitalWrite(this->latchPin, HIGH);
    }

  private:
    int latchPin, dataPin, clockPin;
    byte displayBuffer[8];
};

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

      return distance;
    }
};

// Initialize the LedMatrix object
LedMatrix ledMatrix(LATCH_PIN, DATA_PIN, CLOCK_PIN);

// Initialize DistanceSensor object
DistanceSensor sensor(TRIG_PIN, ECHO_PIN);

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  sensor.begin();
}

void loop() {
  int distance = sensor.getDistance();
  
  // Debugging output for distance
  Serial.print("Distance: ");
  Serial.println(distance);
  
  // Display the distance on the LED matrix
  ledMatrix.clear();
  
  // Calculate mask based on distance
  byte rowMask = 0;
  if (distance >= 0 && distance <= 70) {
    int rowsToLight = distance / 10 + 1; // Scale distance to number of rows
    for (int i = 0; i < rowsToLight; i++) {
      rowMask |= (1 << (7 - i));
    }
  }
  
  for (int col = 0; col < 8; col++) {
    ledMatrix.drawColumn(col, rowMask);
  }

  ledMatrix.update();
  delay(1000); // Update every second
}

#include <Arduino.h>

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
        displayBuffer[7 - column] = mask;
      }
    }

    void update() {
      digitalWrite(this->latchPin, LOW);
      for (byte i = 0; i < 8; i++) {
        shiftOut(this->dataPin, this->clockPin, LSBFIRST, displayBuffer[i]);
        shiftOut(this->dataPin, this->clockPin, LSBFIRST, 1 << i);
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

// DibsE class
class DibsE {
  private:
    DistanceSensor sensor;
    LedMatrix& ledMatrix;

  public:
    DibsE(DistanceSensor sensor, LedMatrix& ledMatrix) : sensor(sensor), ledMatrix(ledMatrix) {}

    void begin() {
      sensor.begin();
      // Initialize RGB LED pins as outputs
      pinMode(RGB_RED_PIN, OUTPUT);
      pinMode(RGB_GREEN_PIN, OUTPUT);
      pinMode(RGB_BLUE_PIN, OUTPUT);
      // Initialize the LED matrix
      ledMatrix.clear();
    }

    void update() {
      int distance = sensor.getDistance();
    
      // Debugging output for distance
      Serial.print("Distance for LED Matrix: ");
      Serial.println(distance);
    
      // Control RGB LED based on distance
      int blinkRate = map(distance, 0, 200, 100, 1000); 
      digitalWrite(RGB_RED_PIN, HIGH);
      delay(blinkRate / 2);
      digitalWrite(RGB_RED_PIN, LOW);
      delay(blinkRate / 2);
    
      // Update LED Matrix
      static byte previousColumns[8] = {0};
    
      // Shift previous columns to the right
      for (int i = 7; i > 0; --i) {
        previousColumns[i] = previousColumns[i - 1];
      }
    
      // Map the distance to the LED matrix height (inverted)
      byte newColumn = map(distance, 0, 70, 0, 7); 
      if (newColumn > 7) newColumn = 7; // Ensure the column index is within bounds
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
};

// Initialize the LedMatrix object
LedMatrix ledMatrix(LATCH_PIN, DATA_PIN, CLOCK_PIN);

// Initialize DistanceSensor and DibsE objects
DistanceSensor sensor(TRIG_PIN, ECHO_PIN);
DibsE dibse(sensor, ledMatrix);

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  dibse.begin();
}

void loop() {
  dibse.update();
  delay(1000); // Update every second
}

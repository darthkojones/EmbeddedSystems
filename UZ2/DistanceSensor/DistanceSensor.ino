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
        displayBuffer[column] = mask;
      }
    }

    void update() {
      digitalWrite(this->latchPin, LOW);
      for (byte i = 0; i < 8; i++) {
        shiftOut(this->dataPin, this->clockPin, LSBFIRST, displayBuffer[i]);
        shiftOut(this->dataPin, this->clockPin, LSBFIRST, 1 << i);
        digitalWrite(this->latchPin, HIGH);  // Move latch HIGH inside the loop
        delayMicroseconds(1);                // Small delay to ensure latch timing
        digitalWrite(this->latchPin, LOW);   // Move latch LOW inside the loop
      }

      // Debugging output for the update function
      Serial.println("LED Matrix Updated:");
      for (byte i = 0; i < 8; i++) {
        Serial.print("Row ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(displayBuffer[i], BIN);
      }
    }

    byte* getBuffer() {
      return displayBuffer;
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

      // Debugging distance calculation
      Serial.print("Duration: ");
      Serial.println(duration);
      Serial.print("Calculated Distance: ");
      Serial.println(distance);

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
      Serial.print("Distance: ");
      Serial.println(distance);

      // Break down the distance into hundreds, tens, and units
      int hundreds = distance / 100;
      int tens = (distance % 100) / 10;
      int units = distance % 10;

      // Debugging output for digits
      Serial.print("Hundreds: ");
      Serial.println(hundreds);
      Serial.print("Tens: ");
      Serial.println(tens);
      Serial.print("Units: ");
      Serial.println(units);

      // Clear the display buffer
      ledMatrix.clear();
      
      // Display hundreds in the 1st column
      ledMatrix.drawColumn(7, createBitmask(hundreds));
      
      // Display tens in the 3rd column
      ledMatrix.drawColumn(5, createBitmask(tens));
      
      // Display units in the 5th column
      ledMatrix.drawColumn(3, createBitmask(units));
    
      // Update the LED matrix display
      ledMatrix.update();

      // Debugging output for display buffer
      for (int i = 0; i < 8; i++) {
        Serial.print("Column ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(ledMatrix.getBuffer()[i], BIN);
      }
    }

    byte createBitmask(int value) {
      byte mask = 0;
      for (int i = 0; i < value; i++) {
        mask |= (1 << i);
      }
      Serial.print("Bitmask for value ");
      Serial.print(value);
      Serial.print(": ");
      Serial.println(mask, BIN);
      return mask;
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
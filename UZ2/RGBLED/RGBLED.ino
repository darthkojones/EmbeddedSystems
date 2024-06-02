#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Define the pins for the RGB LED and the LED Matrix
#define LATCH_PIN 11
#define DATA_PIN 16
#define CLOCK_PIN 15

// Define the pins for the RGB LED
#define RGB_PIN 40  // Updated to pin 40 which corresponds to A4

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

// DibsE class for RGB LED control
class DibsE {
  private:
    Adafruit_NeoPixel rgbLed;
    unsigned long rgbLedTimer;
    unsigned long rgbLedTimeout;
    int32_t rgbLedColor;
    bool rgbLedStatus;

  public:
    DibsE(int pin) : rgbLed(1, pin, NEO_GRB + NEO_KHZ800), rgbLedTimer(0), rgbLedTimeout(0), rgbLedStatus(false) {
      rgbLed.begin();
      rgbLed.show(); // Initialize all pixels to 'off'
    }

    void setColor(int red, int green, int blue) {
      rgbLed.setPixelColor(0, rgbLed.Color(red, green, blue));
      rgbLed.show();
      Serial.print("Setting color to: R=");
      Serial.print(red);
      Serial.print(", G=");
      Serial.print(green);
      Serial.print(", B=");
      Serial.println(blue);
    }
};

DistanceSensor sensor(TRIG_PIN, ECHO_PIN);
DibsE rgbLed(RGB_PIN);

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  sensor.begin();
  rgbLed.setColor(0, 0, 0); // Ensure the LED is off initially
}

void loop() {
  int distance = sensor.getDistance();

  // Set the color of the RGB LED based on distance
  if (distance < 20) {
    rgbLed.setColor(255, 0, 0); // Red
    Serial.println("Setting LED to Red");
  } else if (distance < 40) {
    rgbLed.setColor(255, 165, 0); // Orange
    Serial.println("Setting LED to Orange");
  } else {
    rgbLed.setColor(0, 255, 0); // Green
    Serial.println("Setting LED to Green");
  }

  delay(1000); // Update every second
}

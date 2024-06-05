#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Define the pin for the RGB LED
#define RGB_LED_PIN A4
#define PIR_PIN 5

// Define the light sensor and joystick pins
#define LIGHT_SENSOR_PIN A2
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_BUTTON_PIN 2

// Adafruit_NeoPixel object for controlling the RGB LED
Adafruit_NeoPixel rgbLed(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

// Light Sensor class
class Olimex_Light_Sensor {
  private:
    int Pin;
  public:
    Olimex_Light_Sensor(int _Pin) : Pin(_Pin) {
      pinMode(Pin, INPUT);
    }
    int Read() {
      return analogRead(Pin);
    }
    float ReadPercentage() {
      return Read() * 100.0 / 1023.0;
    }
};

// Joystick class
class Olimex_Joystick {
  private:
    int xPin, yPin, butPin;
  public:
    Olimex_Joystick(int x, int y, int but) : xPin(x), yPin(y), butPin(but) {
      pinMode(butPin, INPUT);
    }
    int X() {
      return analogRead(xPin);
    }
    int Y() {
      return analogRead(yPin);
    }
    bool But() {
      return digitalRead(butPin) == LOW;
    }
};

// Initialize sensor objects
Olimex_Light_Sensor lightSensor(LIGHT_SENSOR_PIN);
Olimex_Joystick joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, JOYSTICK_BUTTON_PIN);

class Ampel {
  private:
    enum State { RED, YELLOW, GREEN, BLINKING_GREEN } state;
    unsigned long stateStartTime;
    unsigned long blinkStartTime;
    unsigned long pedestrianCooldownTime;
    unsigned long emergencyCooldownTime;
    unsigned long nightModeCooldownTime;
    bool isNightMode;

  public:
    Ampel() : state(RED), stateStartTime(0), blinkStartTime(0), pedestrianCooldownTime(0), emergencyCooldownTime(0), nightModeCooldownTime(0), isNightMode(false) {}

    void begin() {
      rgbLed.begin();
      pinMode(PIR_PIN, INPUT);
      setColor(255, 0, 0); // Start with RED
    }

    void setColor(int red, int green, int blue) {
      rgbLed.setPixelColor(0, rgbLed.Color(red, green, blue));
      rgbLed.show();
    }

    void update() {
      unsigned long currentMillis = millis();

      if (isNightMode) {
        if (currentMillis - stateStartTime >= 500) {
          stateStartTime = currentMillis;
          if (rgbLed.getPixelColor(0) == rgbLed.Color(255, 255, 0)) {
            setColor(0, 0, 0); // Turn off
          } else {
            setColor(255, 255, 0); // Yellow
          }
        }
        return;
      }

      switch (state) {
        case RED:
          if (currentMillis - stateStartTime >= 7000) {
            state = YELLOW;
            stateStartTime = currentMillis;
            setColor(255, 255, 0); // Yellow
            Serial.println("State: YELLOW");
          }
          break;

        case YELLOW:
          if (currentMillis - stateStartTime >= 3000) {
            state = GREEN;
            stateStartTime = currentMillis;
            setColor(0, 255, 0); // Green
            Serial.println("State: GREEN");
          }
          break;

        case GREEN:
          if (currentMillis - stateStartTime >= 5000) {
            state = BLINKING_GREEN;
            stateStartTime = currentMillis;
            blinkStartTime = currentMillis;
            Serial.println("State: BLINKING_GREEN");
          }
          break;

        case BLINKING_GREEN:
          if (currentMillis - stateStartTime >= 2000) {
            state = RED;
            stateStartTime = currentMillis;
            setColor(255, 0, 0); // Red
            Serial.println("State: RED");
          } else if (currentMillis - blinkStartTime >= 500) {
            blinkStartTime = currentMillis;
            if (rgbLed.getPixelColor(0) == rgbLed.Color(0, 255, 0)) {
              setColor(0, 0, 0); // Turn off
            } else {
              setColor(0, 255, 0); // Green
            }
          }
          break;
      }
    }

    void checkNightMode() {
      unsigned long currentMillis = millis();
      float lightLevel = lightSensor.ReadPercentage();

      if (currentMillis - nightModeCooldownTime > 1000) {
        if (lightLevel < 10.0) {
          if (!isNightMode) {
            isNightMode = true;
            stateStartTime = millis();
            setColor(255, 255, 0); // Yellow
            Serial.println("Night mode activated");
          }
        } else if (lightLevel > 20.0) {
          if (isNightMode) {
            isNightMode = false;
            state = RED;
            stateStartTime = millis();
            setColor(255, 0, 0); // Red
            Serial.println("Day mode activated");
          }
        }
        nightModeCooldownTime = currentMillis;
      }
    }

    void checkPedestrian() {
      unsigned long currentMillis = millis();
      if (digitalRead(PIR_PIN) == HIGH && currentMillis - pedestrianCooldownTime > 5000) {
        pedestrianCooldownTime = currentMillis;
        Serial.println("Pedestrian detected");
        if (state == GREEN || state == BLINKING_GREEN) {
          state = YELLOW;
          stateStartTime = currentMillis;
          setColor(255, 255, 0); // Yellow
          Serial.println("Switching to YELLOW");
        } else if (state == YELLOW) {
          state = RED;
          stateStartTime = currentMillis;
          setColor(255, 0, 0); // Red
          Serial.println("Switching to RED");
        } else if (state == RED) {
          stateStartTime += 5000; // Extend RED phase by 5 seconds
          Serial.println("Extending RED phase by 5 seconds");
        }
      }
    }

    void checkEmergencyVehicle() {
      unsigned long currentMillis = millis();
      if ((joystick.X() > 600 || joystick.Y() > 600 || joystick.X() < 400 || joystick.Y() < 400) && currentMillis - emergencyCooldownTime > 5000) {
        emergencyCooldownTime = currentMillis;
        Serial.println("Emergency vehicle detected");
        if (state == RED) {
          state = YELLOW;
          stateStartTime = currentMillis;
          setColor(255, 255, 0); // Yellow
          Serial.println("Switching to YELLOW");
        } else if (state == GREEN || state == BLINKING_GREEN) {
          stateStartTime += 5000; // Extend GREEN phase by 5 seconds
          Serial.println("Extending GREEN phase by 5 seconds");
        }
      }
    }
};

// Initialize Ampel object
Ampel ampel;

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  ampel.begin();
}

void loop() {
  ampel.checkNightMode();
  ampel.checkPedestrian();
  ampel.checkEmergencyVehicle();
  ampel.update();
  delay(10); // Small delay to avoid excessive CPU usage
}

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

// Olimex_Light_Sensor class definition
class Olimex_Light_Sensor {
  private:
    int Pin;
  public:
    Olimex_Light_Sensor(int _Pin) : Pin(_Pin) {
      pinMode(Pin, INPUT_PULLUP);
    }
    int Read() {
      return analogRead(Pin);
    }
    float ReadPercentage() {
      return Read() * 100.0 / 1023.0;
    }
};

// Olimex_Joystick class definition
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

// Initialize sensors
Olimex_Light_Sensor lightSensor(LIGHT_SENSOR_PIN);
Olimex_Joystick joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, JOYSTICK_BUTTON_PIN);

class Ampel {
  private:
    enum State { RED, YELLOW, GREEN, BLINK_GREEN, NIGHT_MODE } state;
    unsigned long lastStateChange;
    int blinkInterval;
    bool isBlinkOn;

  public:
    Ampel() : state(RED), lastStateChange(0), blinkInterval(500), isBlinkOn(false) {}

    void setup() {
      pinMode(PIR_PIN, INPUT);
      rgbLed.begin();
      rgbLed.show(); // Initialize all pixels to 'off'
      setColor(255, 0, 0); // Start with RED
    }

    void update() {
      unsigned long currentMillis = millis();

      switch (state) {
        case RED:
          if (currentMillis - lastStateChange >= 7000) {
            setColor(255, 255, 0); // Switch to YELLOW
            state = YELLOW;
            lastStateChange = currentMillis;
          }
          break;

        case YELLOW:
          if (currentMillis - lastStateChange >= 3000) {
            setColor(0, 255, 0); // Switch to GREEN
            state = GREEN;
            lastStateChange = currentMillis;
          }
          break;

        case GREEN:
          if (currentMillis - lastStateChange >= 5000) {
            state = BLINK_GREEN;
            lastStateChange = currentMillis;
          }
          break;

        case BLINK_GREEN:
          if (currentMillis - lastStateChange >= blinkInterval) {
            isBlinkOn = !isBlinkOn;
            setColor(isBlinkOn ? 0 : 0, isBlinkOn ? 255 : 0, 0); // Blink GREEN
            lastStateChange = currentMillis;
            if (!isBlinkOn) {
              blinkInterval -= 500;
              if (blinkInterval <= 0) {
                setColor(255, 255, 0); // Switch to YELLOW
                state = YELLOW;
                lastStateChange = currentMillis;
                blinkInterval = 500;
              }
            }
          }
          break;

        case NIGHT_MODE:
          if (currentMillis - lastStateChange >= 500) {
            isBlinkOn = !isBlinkOn;
            setColor(isBlinkOn ? 255 : 0, isBlinkOn ? 255 : 0, 0); // Blink YELLOW
            lastStateChange = currentMillis;
          }
          break;
      }
    }

    void handlePedestrian() {
      if (state == GREEN || state == BLINK_GREEN) {
        setColor(255, 255, 0); // Switch to YELLOW
        state = YELLOW;
        lastStateChange = millis();
      } else if (state == RED) {
        // Extend RED phase
        lastStateChange -= 5000;
      }
    }

    void handleEmergency() {
      if (state == RED) {
        setColor(255, 255, 0); // Switch to YELLOW
        state = YELLOW;
        lastStateChange = millis();
      } else if (state == GREEN) {
        // Extend GREEN phase
        lastStateChange -= 5000;
      }
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

    void checkNightMode() {
      if (lightSensor.ReadPercentage() < 10.0) {
        if (state != NIGHT_MODE) {
          state = NIGHT_MODE;
          lastStateChange = millis();
        }
      } else if (state == NIGHT_MODE) {
        state = RED;
        setColor(255, 0, 0); // Switch to RED
        lastStateChange = millis();
      }
    }
};

Ampel ampel;

void setup() {
  Serial.begin(9600);
  ampel.setup();
}

void loop() {
  ampel.update();
  ampel.checkNightMode();

  // Check for pedestrian detection
  if (digitalRead(PIR_PIN) == HIGH) {
    ampel.handlePedestrian();
  }

  // Check for emergency vehicle detection
  if (joystick.But()) {
    ampel.handleEmergency();
  }
}

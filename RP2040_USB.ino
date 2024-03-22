#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Keyboard.h>

const int BUTTON_GPIO = D0; // GPIO pin for the button input
unsigned long lastButtonPressTime = 0;
unsigned long lastLongHoldReportTime = 0;
bool longHoldDetected = false;
bool shortHoldDetected = false;

int Power = 11;
int PIN  = 12;
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(BUTTON_GPIO, INPUT_PULLDOWN); // Set button input pin with internal pull-down resistor
  pinMode(Power, OUTPUT);
  digitalWrite(Power, HIGH);
  pixels.begin(); // Initialize NeoPixel
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel initially
  pixels.show();
  Keyboard.begin(); // Initialize USB Keyboard
}

void loop() {
  if (digitalRead(BUTTON_GPIO) == HIGH && millis() - lastButtonPressTime > 100) { // Button is pressed and debounce timer (100ms) has elapsed
    unsigned long buttonPressStartTime = millis();

    // Wait for button release or long hold
    while (digitalRead(BUTTON_GPIO) == HIGH) {
      if (millis() - buttonPressStartTime > 1500) { // Long hold (> 1500ms)
        if (!longHoldDetected) {
          Serial.println("Long hold");
          pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Turn NeoPixel red
          pixels.show();
          Keyboard.press(KEY_UP_ARROW);
          delay(50);
          Keyboard.release(KEY_UP_ARROW);
          longHoldDetected = true;
          lastLongHoldReportTime = millis();
          break;
        }
        else if (millis() - lastLongHoldReportTime >= 1000) {
          Serial.println("Long hold continued");
          Keyboard.press(KEY_UP_ARROW);
          delay(50);
          Keyboard.release(KEY_UP_ARROW);
          lastLongHoldReportTime = millis();
        }
      }
    }

    if (!longHoldDetected && digitalRead(BUTTON_GPIO) == LOW) { // Short hold (< 2500ms)
      delay(100); // Wait for debounce
      if (digitalRead(BUTTON_GPIO) == LOW && !shortHoldDetected) { // Button is released
        Serial.println("Short hold");
        pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Turn NeoPixel green
        pixels.show();
        Keyboard.press(KEY_DOWN_ARROW); // Send down arrow through keyboard
        delay(50);
        Keyboard.release(KEY_DOWN_ARROW);
        delay(1000); // Wait for 1 second
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel
        pixels.show();
        shortHoldDetected = true;
      }
    }

    lastButtonPressTime = millis(); // Update last button press time
  }
  else if(digitalRead(BUTTON_GPIO) == LOW){ // Button is not pressed, turn off lights
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel
    pixels.show();
    longHoldDetected = false;
    Keyboard.releaseAll(); // Release all keys
    shortHoldDetected = false;
  }
}

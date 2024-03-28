#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Keyboard.h>

const int BUTTON_GPIO = D0; // GPIO pin for the button input
unsigned long lastButtonPressTime = 0;
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
  if (digitalRead(BUTTON_GPIO) == HIGH && millis() - lastButtonPressTime > 50) { // Button is pressed and debounce timer (50ms) has elapsed
    unsigned long buttonPressStartTime = millis();
    
    // Check for long hold (> 750ms)
    while (digitalRead(BUTTON_GPIO) == HIGH && millis() - buttonPressStartTime < 750) {
      delay(50); // Delay to debounce and avoid multiple detections
    }
    
    if (digitalRead(BUTTON_GPIO) == HIGH) {
      Serial.println("Long hold");
      pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Turn NeoPixel red
      pixels.show();
      Keyboard.press(KEY_UP_ARROW);
      delay(50);
      Keyboard.release(KEY_UP_ARROW);
      longHoldDetected = true;
      shortHoldDetected = true;
    } else {
      // Short hold (< 750ms)
      if (!shortHoldDetected) {
        Serial.println("Short hold");
        pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Turn NeoPixel green
        pixels.show();
        Keyboard.press(KEY_DOWN_ARROW); // Send down arrow through keyboard
        delay(50);
        Keyboard.release(KEY_DOWN_ARROW);
        shortHoldDetected = true;
      }
    }

    // Update last button press time
    lastButtonPressTime = millis();
  } else if (digitalRead(BUTTON_GPIO) == LOW) { // Button is not pressed, turn off lights
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel
    pixels.show();
    longHoldDetected = false;
    shortHoldDetected = false;
    Keyboard.releaseAll(); // Release all keys
    
    // Introduce a small delay before checking button state again
    delay(50);
  }
}

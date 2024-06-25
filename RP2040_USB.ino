#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Keyboard.h>

const int BUTTON_FORWARD_GPIO = D1; // GPIO pin for the forward button input
const int BUTTON_BACKWARD_GPIO = D0; // GPIO pin for the backward button input
unsigned long lastForwardPressTime = 0;
unsigned long lastBackwardPressTime = 0;

int Power = 11;
int PIN  = 12;
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(BUTTON_FORWARD_GPIO, INPUT_PULLDOWN); // Set forward button input pin with internal pull-down resistor
  pinMode(BUTTON_BACKWARD_GPIO, INPUT_PULLDOWN); // Set backward button input pin with internal pull-down resistor
  pinMode(Power, OUTPUT);
  digitalWrite(Power, HIGH);
  pixels.begin(); // Initialize NeoPixel
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel initially
  pixels.show();
  Keyboard.begin(); // Initialize USB Keyboard
}

void loop() {
  // Handle forward button press
  if (digitalRead(BUTTON_FORWARD_GPIO) == HIGH && millis() - lastForwardPressTime > 50) { // Forward button is pressed and debounce timer (50ms) has elapsed
    handleButtonPress(BUTTON_FORWARD_GPIO, KEY_UP_ARROW, pixels.Color(0, 0, 255)); // Blue flash for forward button
    lastForwardPressTime = millis();
  }

  // Handle backward button press
  if (digitalRead(BUTTON_BACKWARD_GPIO) == HIGH && millis() - lastBackwardPressTime > 50) { // Backward button is pressed and debounce timer (50ms) has elapsed
    handleButtonPress(BUTTON_BACKWARD_GPIO, KEY_DOWN_ARROW, pixels.Color(255, 0, 0)); // Red flash for backward button
    lastBackwardPressTime = millis();
  }

  // Turn off NeoPixel when buttons are not pressed
  if (digitalRead(BUTTON_FORWARD_GPIO) == LOW && digitalRead(BUTTON_BACKWARD_GPIO) == LOW) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel
    pixels.show();
    Keyboard.releaseAll(); // Release all keys
    delay(50); // Small delay before checking button state again
  }
}

void handleButtonPress(int buttonPin, uint8_t key, uint32_t color) {
  unsigned long buttonPressStartTime = millis();

  // Check for long hold (> 750ms)
  while (digitalRead(buttonPin) == HIGH && millis() - buttonPressStartTime < 750) {
    delay(50); // Delay to debounce and avoid multiple detections
  }

  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Long hold");
    pixels.setPixelColor(0, color); // Set NeoPixel to the specified color
    pixels.show();
    Keyboard.press(key);
    delay(50);
    Keyboard.release(key);
  } else {
    Serial.println("Short hold");
    pixels.setPixelColor(0, color); // Set NeoPixel to the specified color
    pixels.show();
    Keyboard.press(key);
    delay(50);
    Keyboard.release(key);
  }
}

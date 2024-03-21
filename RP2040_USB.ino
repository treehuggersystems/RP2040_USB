#include <Arduino.h>
#include <Keyboard.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 25 // The built-in LED pin on the XIAO RP2040
#define NEOPIXEL_PIN 22 // NeoPixel pin
#define NEOPIXEL_COUNT 1 // Number of NeoPixels

#define DOWN_ARROW_GPIO 2 // GPIO pin for Down Arrow input
#define UP_ARROW_GPIO 3 // GPIO pin for Up Arrow input

Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
  digitalWrite(LED_PIN, LOW); // Turn off the LED initially

  pinMode(DOWN_ARROW_GPIO, INPUT_PULLUP); // Set Down Arrow input pin
  pinMode(UP_ARROW_GPIO, INPUT_PULLUP); // Set Up Arrow input pin

  pixels.begin(); // Initialize NeoPixel

  // Initialize USB Keyboard
  Keyboard.begin();
  delay(2000); // delay 2 seconds for setup
}

void loop() {
  static bool downArrowPressed = false;
  static bool upArrowPressed = false;

  // Check for rising edge of Down Arrow input
  if (digitalRead(DOWN_ARROW_GPIO) == LOW && !downArrowPressed) {
    Keyboard.press(KEY_DOWN_ARROW); // Press Down Arrow key
    delay(100); // Optional delay for key press
    Keyboard.release(KEY_DOWN_ARROW); // Release Down Arrow key
    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Set NeoPixel to Red
    pixels.show();
    downArrowPressed = true;
  }
  else if (digitalRead(DOWN_ARROW_GPIO) == HIGH && downArrowPressed) {
    downArrowPressed = false;
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel
    pixels.show();
  }

  // Check for rising edge of Up Arrow input
  if (digitalRead(UP_ARROW_GPIO) == LOW && !upArrowPressed) {
    Keyboard.press(KEY_UP_ARROW); // Press Up Arrow key
    delay(100); // Optional delay for key press
    Keyboard.release(KEY_UP_ARROW); // Release Up Arrow key
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Set NeoPixel to Green
    pixels.show();
    upArrowPressed = true;
  }
  else if (digitalRead(UP_ARROW_GPIO) == HIGH && upArrowPressed) {
    upArrowPressed = false;
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turn off NeoPixel
    pixels.show();
  }

  // Wait for a moment before checking inputs again
  delay(100);
}

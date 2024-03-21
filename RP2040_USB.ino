#include <Arduino.h>
#include <Keyboard.h>

#define LED_PIN 25 // The built-in LED pin on the XIAO RP2040

void setup() {
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
  digitalWrite(LED_PIN, HIGH); // Turn on the LED

  // Initialize USB Keyboard
  Keyboard.begin();
  delay(2000); // delay 2 seconds for setup
}

void loop() {
  Keyboard.press(KEY_DOWN_ARROW); // Press Down Arrow key
  delay(100); // Optional delay for key press
  Keyboard.release(KEY_DOWN_ARROW); // Release Down Arrow key

  delay(1000);

  Keyboard.press(KEY_DOWN_ARROW); // Press Down Arrow key
  delay(100); // Optional delay for key press
  Keyboard.release(KEY_DOWN_ARROW); // Release Down Arrow key

  delay(1000);

  Keyboard.press(KEY_UP_ARROW); // Press Up Arrow key
  delay(100); // Optional delay for key press
  Keyboard.release(KEY_UP_ARROW); // Release Up Arrow key
  
  // Wait for a moment before typing again
  delay(5000);
}

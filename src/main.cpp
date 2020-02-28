#include <Arduino.h>

#define NUM_BUTTONS 11
uint8_t buttons[NUM_BUTTONS] = {10, 21, 19, 18, 15, 4, 27, 14, 13, 9, 0};
uint8_t pressed[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW};
char values[NUM_BUTTONS] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'E',
};

void setupButtons() {
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }
}

void setup() { Serial.begin(115200); }

void loop() {
  delay(1000);
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    if (digitalRead(buttons[i]) == pressed[i]) {
      Serial.println(values[i]);
    }
  }
}
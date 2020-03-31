#include <Arduino.h>
#include <BleKeyboard.h>
#include <esp_wifi.h>

BleKeyboard bleKeyboard = BleKeyboard("Huemote", "p3dt", 100);

#define MESSAGE_DELAY_MS 1000
#define KEY_DELAY_MS 250

#define NUM_BUTTONS 11
uint8_t buttons[NUM_BUTTONS] = {10, 21, 19, 18, 15, 4, 27, 14, 13, 9, 0};
uint8_t pressed[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW};
char values[NUM_BUTTONS] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', KEY_RETURN,
};

void setupButtons() {
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }
}

void setMinCPUSpeed() {
  // https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-cpu.h
  // ESP32 PICO D4 -> https://docs.espressif.com/projects/esp-idf/en/latest/hw-reference/get-started-pico-kit.html
  // -> 40MHz Oscillator
  // //  240, 160, 80, 40, 20, 10  <<< For 40MHz XTAL
  setCpuFrequencyMhz(80);
  Serial.print("Setting CPU to ");
  Serial.print(80);
  Serial.println(" MHz");
}

void setup() {
  // esp_wifi_stop();
  Serial.begin(115200);

  setMinCPUSpeed();
  setupButtons();
  bleKeyboard.begin();
}

void loop() {
  static unsigned long lastMessage = 0;
  static unsigned long lastKeyPress = 0;
  if (lastMessage == 0) {
    lastMessage = millis();
  }
  if (lastKeyPress == 0) {
    lastKeyPress = millis();
  }

  delay(10);

  if (lastMessage + MESSAGE_DELAY_MS < millis()) {
    Serial.println(bleKeyboard.isConnected() ? "Connected" : "Disconnected");
    lastMessage = millis();
  }

  if (lastKeyPress + KEY_DELAY_MS < millis()) {
    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
      delay(1);
      if (digitalRead(buttons[i]) == pressed[i]) {
        Serial.println(values[i]);
        if (bleKeyboard.isConnected()) {
          if (values[i] == KEY_RETURN) {
            bleKeyboard.print((char)KEY_TAB);
            delay(250);
            bleKeyboard.print((char)KEY_TAB);
            delay(250);
            bleKeyboard.print((char)KEY_RETURN);
          } else {
            bleKeyboard.print(values[i]);
          }
        }
        break;
      }
    }
    lastKeyPress = millis();
  }
}
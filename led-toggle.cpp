#include <Arduino.h>
#include "led-toggle.h"

const int heartbeatLed = D10;
const unsigned long LED_HALF_CYCLE_TIME = 25;
unsigned long lastLedToggle = 0;
boolean ledIsOn = false;

// PUBLIC FUNCTIONS

void setupHeartbeatLed() {
  // set up LED output for visual heartbeat and test purposes
  pinMode(heartbeatLed, OUTPUT);
  ledIsOn = false;
  lastLedToggle = millis();
}

void handleHeartbeatLed() {
  unsigned long now = millis();
  if ((now - lastLedToggle) >= LED_HALF_CYCLE_TIME) {
    lastLedToggle = now;
    ledIsOn = !ledIsOn;
    if (ledIsOn) {
      digitalWrite(heartbeatLed, HIGH);
    } else {
      digitalWrite(heartbeatLed, LOW);
    }
  }
}

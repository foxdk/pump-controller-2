#include <Arduino.h>
#include "pump-control.h"
#include "logger.h"

const int pumpRelayPin = D4;

const unsigned long PUMP_RUN_DURATION_MILLIS = 90 * 1000;
const unsigned long MIN_WAIT_AFTER_LARGE_FLOW_MILLIS = 20 * 60 * 1000;  // let water cool before running again
const unsigned long LARGE_FLOW_MIN_DURATION_MILLIS = 60 * 1000;

unsigned long pumpStartMillis = 0;
unsigned long flowBeginMillis = 0;
unsigned long largeFlowEndMillis = 0;
bool pumpIsOn = false;

void startPump() {
  pumpStartMillis = millis();
  pumpIsOn = true;
  digitalWrite(pumpRelayPin, HIGH);
  logPrintln("Pump ON");
}

void stopPump(bool enableLogging) {
  digitalWrite(pumpRelayPin, LOW);
  pumpIsOn = false;
  if (enableLogging) {
    logPrintln("Pump off");
  }
}

// PUBLIC FUNCTIONS

void setupPumpControl() {
  pinMode(pumpRelayPin, OUTPUT);
  stopPump(false);
  pumpStartMillis = 0;
  flowBeginMillis = 0;
  largeFlowEndMillis = 0; // pump will not start for the first time until after MIN_WAIT_AFTER_LARGE_FLOW_MILLIS, for added safety
}

void flowBegin() {
  unsigned long now = millis();
  flowBeginMillis = now;
  if (!pumpIsOn && (now - largeFlowEndMillis) >= MIN_WAIT_AFTER_LARGE_FLOW_MILLIS) {
    startPump();
  }
}

void flowEnd() {
  unsigned long now = millis();
  unsigned long duration = now - flowBeginMillis;
  if (duration >= LARGE_FLOW_MIN_DURATION_MILLIS) {
    largeFlowEndMillis = now;
  }
  logFlowln(duration);
}

void handlePumpControl() {
  unsigned long now = millis();
  if (pumpIsOn && (now - pumpStartMillis) >= PUMP_RUN_DURATION_MILLIS) {
    stopPump(true);
  }
}

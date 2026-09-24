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
boolean pumpIsOn = false;

void startPump() {
  digitalWrite(pumpRelayPin, HIGH);
  pumpIsOn = true;
  pumpStartMillis = millis();
  logPrintln("Pump ON");
}

void stopPump(unsigned long now) {
  largeFlowEndMillis = now;
  digitalWrite(pumpRelayPin, LOW);
  pumpIsOn = false;
  if (now != 0UL) {
    logPrintln("Pump off");
  }
}

// PUBLIC FUNCTIONS

void setupPumpControl() {
  pinMode(pumpRelayPin, OUTPUT);
  stopPump(0UL);
  pumpStartMillis = 0;
  flowBeginMillis = 0;
  largeFlowEndMillis = millis() - MIN_WAIT_AFTER_LARGE_FLOW_MILLIS;  // start pump on first flow after reboot
}

void flowBegin() {
  flowBeginMillis = millis();
  if (!pumpIsOn && (flowBeginMillis - largeFlowEndMillis) >= MIN_WAIT_AFTER_LARGE_FLOW_MILLIS) {
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
    stopPump(now);
  }
}

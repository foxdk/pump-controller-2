#include <Arduino.h>
#include "pump-control.h"
#include "logger.h"

const int pumpRelayPin = D4;

const unsigned long PUMP_RUN_DURATION_MILLIS = 90 * 1000;
const unsigned long MIN_WAIT_AFTER_LARGE_FLOW_MILLIS = 20 * 60 * 1000;  // let water and pump cool before running again
const unsigned long LARGE_FLOW_MIN_DURATION_MILLIS = 60 * 1000;

unsigned long pumpStartMillis = 0;
unsigned long pumpStopMillis = 0;
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
  pumpStopMillis = millis();
  pumpIsOn = false;
  if (enableLogging) {
    logPrintln("Pump off");
  }
}

// PUBLIC FUNCTIONS

void setupPumpControl() {
  pinMode(pumpRelayPin, OUTPUT);
  stopPump(false);
  // Setting pumpStopMillis and largeFlowEndMillis to 0 here means that each controller restart will initially
  // prevent the pump from running until after MIN_WAIT_AFTER_LARGE_FLOW_MILLIS, for added safety.
  pumpStartMillis = 0;
  pumpStopMillis = 0;
  flowBeginMillis = 0;
  largeFlowEndMillis = 0;
}

void flowBegin() {
  unsigned long now = millis();
  flowBeginMillis = now;
  // We don't want to start the pump yet if either the pump has run recently or we've had a large flow recently.
  // Generally, the pump running will always cause a large flow, unless someone changes PUMP_RUN_DURATION_MILLIS
  // to be less than LARGE_FLOW_MIN_DURATION_MILLIS. The check below adds safety around that possibility.
  bool largeFlowRecently = (now - largeFlowEndMillis) < MIN_WAIT_AFTER_LARGE_FLOW_MILLIS;
  bool pumpRanRecently = (now - pumpStopMillis) < MIN_WAIT_AFTER_LARGE_FLOW_MILLIS;
  if (!pumpIsOn && !largeFlowRecently && !pumpRanRecently) {
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

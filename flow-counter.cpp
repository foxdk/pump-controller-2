#include <Arduino.h>
#include "flow-counter.h"
#include "pump-control.h"

const unsigned long TIMESLICE_PERIOD = 250;
unsigned long lastTimeslice = 0;

volatile unsigned long currentFlowCount = 0;

const int sensorPin = D3;

const unsigned long FLOW_DETECTED_MIN_COUNT = 4;
bool flowing = false;

void sensorISR()
{
  currentFlowCount++;
}

void setupFlowCounter()
{
  pinMode(sensorPin, INPUT);

  currentFlowCount = 0;
  lastTimeslice = 0;

  flowing = false;

  attachInterrupt(digitalPinToInterrupt(sensorPin), sensorISR, FALLING);
}

void handleFlowCounter()
{
  unsigned long now = millis();
  if ((now - lastTimeslice) >= TIMESLICE_PERIOD)
  {
    lastTimeslice = now;
    unsigned long snapshotCount = currentFlowCount;
    currentFlowCount = 0;
    if (snapshotCount >= FLOW_DETECTED_MIN_COUNT && !flowing)
    {
      flowBegin();
      flowing = true;
    }
    else if (snapshotCount == 0 && flowing)
    {
      flowEnd();
      flowing = false;
    }
  }
}

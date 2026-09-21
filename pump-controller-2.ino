#include <Arduino.h>
#include "led-toggle.h"
#include "flow-counter.h"
#include "wifi-controller.h"
#include "logger.h"
#include "pump-control.h"

void warmUpSerial()
{
  Serial.begin(115200);
  delay(1000); // wait for Serial connection to warm up
  Serial.println(getProgramNameAndVersion());
}

void setup()
{
  initializeLog();
  warmUpSerial();
  setupPumpControl();
  setupHeartbeatLed();
  setupFlowCounter();
  setupWiFi();
}

void loop()
{
  handleHeartbeatLed();
  handleFlowCounter();
  handlePumpControl();
  handleWebClient();
}

#include <Arduino.h>
#include <esp_system.h>
#include <rom/ets_sys.h>
#include "led-toggle.h"
#include "flow-counter.h"
#include "wifi-controller.h"
#include "logger.h"
#include "pump-control.h"

const int PUMP_CTRL_WATCHDOG_TIMEOUT = 10 * 1000;  // 10 seconds without feeding watchdog = reboot
hw_timer_t *watchdogTimer = NULL;

void ARDUINO_ISR_ATTR restartPumpController() {
  esp_restart();
}

void setupWatchdogTimer() {
  watchdogTimer = timerBegin(1000000);                                     //timer 1Mhz resolution
  timerAttachInterrupt(watchdogTimer, &restartPumpController);             //attach callback
  timerAlarm(watchdogTimer, PUMP_CTRL_WATCHDOG_TIMEOUT * 1000, false, 0);  //set time in us
}

void warmUpSerial() {
  Serial.begin(115200);
  Serial.println(getProgramNameAndVersion());
}

void setup() {
  setupWatchdogTimer();
  initializeLog();
  warmUpSerial();
  setupPumpControl();
  setupHeartbeatLed();
  setupFlowCounter();
  setupWiFi();
}

void loop() {
  timerWrite(watchdogTimer, 0);  //reset timer (feed watchdog)
  handleHeartbeatLed();
  handleFlowCounter();
  handlePumpControl();
  handleWebClient();
}

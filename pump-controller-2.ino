/*
   Copyright (c) 2026 David K. Fox
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/foxdk/pump-controller-2
 */
#include <Arduino.h>
#include <esp_system.h>
#include <rom/ets_sys.h>
#include "led-toggle.h"
#include "flow-counter.h"
#include "wifi-controller.h"
#include "logger.h"
#include "pump-control.h"
#include "command-line.h"

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
  delay(1000);
  Serial.println(getProgramNameAndVersion());
}

void setup() {
  warmUpSerial();
  setupWatchdogTimer();
  initializeLog();
  setupPumpControl();
  setupHeartbeatLed();
  setupFlowCounter();
  setupWiFi();
  setupCommandLine();
}

void loop() {
  timerWrite(watchdogTimer, 0);  //reset timer (feed watchdog)
  handleHeartbeatLed();
  handleFlowCounter();
  handlePumpControl();
  handleWebClient();
  handleCommandLine();
}

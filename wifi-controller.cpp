/*
   Copyright (c) 2026 David K. Fox
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/foxdk/pump-controller-2
 */
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include "wifi-controller.h"
#include "logger.h"

const int attemptingConnectionLED = D5;

unsigned long lastConnectCheck;
const unsigned long CONNECT_CHECK_INTERVAL = 10000;
bool ipAddressPrinted = false;

const unsigned long MAX_RECONNECT_TRIGGER = 24 * 3600 * 1000 / CONNECT_CHECK_INTERVAL;  // one day max
unsigned long reconnectCounter = 0;
unsigned long reconnectTrigger = 1;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -25200;
const int daylightOffset_sec = 3600;

// Create a web server object listening on port 80 (standard HTTP port)
WebServer server(80);

// Function executed when a client requests the root URL ("/")
void handleRoot() {
  sendToWebServer(server);
}

// Function executed for undefined URLs (404 Error)
void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

void beginWiFi() {
  digitalWrite(attemptingConnectionLED, HIGH);
  Serial.println("wi-fi connect attempt");
  WiFi.begin();
}

void increaseTriggerDelayExponentially() {
  if (reconnectTrigger == 0) {
    reconnectTrigger = 1;
  }
  reconnectTrigger = 2 * reconnectTrigger;
  if (reconnectTrigger > MAX_RECONNECT_TRIGGER) {
    reconnectTrigger = MAX_RECONNECT_TRIGGER;
  }
}

void reconnectIfNeeded() {
  unsigned long now = millis();
  if ((now - lastConnectCheck) >= CONNECT_CHECK_INTERVAL) {
    lastConnectCheck = now;
    if (WiFi.status() == WL_CONNECTED) {
      // Init and get the time
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

      if (!ipAddressPrinted) {
        Serial.print("WiFi connected! IP Address: ");
        Serial.println(WiFi.localIP());
        ipAddressPrinted = true;
        logPrintln("WiFi Initialized");
      }
      digitalWrite(attemptingConnectionLED, LOW);
      reconnectCounter = 0;
      reconnectTrigger = 1;
    } else {
      WiFi.disconnect();
      digitalWrite(attemptingConnectionLED, LOW);
      ipAddressPrinted = false;
      if (++reconnectCounter >= reconnectTrigger) {
        reconnectCounter = 0;
        increaseTriggerDelayExponentially();
        beginWiFi();
      }
    }
  }
}

// **** PUBLIC FUNCTIONS ****

void setupWiFi() {
  pinMode(attemptingConnectionLED, OUTPUT);
  lastConnectCheck = millis() + 3000UL - CONNECT_CHECK_INTERVAL;  // first check at 3 seconds
  ipAddressPrinted = false;

  reconnectCounter = 0;
  reconnectTrigger = 1;

  beginWiFi();

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void handleWebClient() {
  reconnectIfNeeded();

  // Continuously listen for incoming client requests
  server.handleClient();
}

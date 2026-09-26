/*
   Copyright (c) 2026 David K. Fox
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/foxdk/pump-controller-2
 */
#include <SimpleCLI.h>
#include <WiFi.h>

SimpleCLI cli;

// Commands
Command cmdSetUpWiFi;
Command cmdWdSelfTest;
Command cmdHelp;

// PUBLIC FUNCTIONS

void setupCommandLine() {
  cmdSetUpWiFi = cli.addCommand("setupwifi");
  cmdSetUpWiFi.setDescription(" Set up WiFi");
  cmdSetUpWiFi.addPositionalArgument("ssid");
  cmdSetUpWiFi.addPositionalArgument("password");

  cmdWdSelfTest = cli.addCommand("wdselftest");
  cmdWdSelfTest.setDescription(" Run a watchdog self-test by going into an infinite loop (will cause reboot)");

  cmdHelp = cli.addCommand("help");
  cmdHelp.setDescription(" Show a description of each command");

  Serial.println("Enter 'help' at the console to see available commands");
  Serial.println();
}

void handleCommandLine() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (input.length() > 0) {
      cli.parse(input);
    }
  }

  if (cli.available()) {
    Command c = cli.getCmd();

    Serial.println();

    if (c == cmdSetUpWiFi) {
      String ssidToSet = c.getArg(0).getValue();
      String passwordToSet = c.getArg(1).getValue();
      Serial.println("Saving credentials and attempting to connect");
      WiFi.persistent(true);
      WiFi.begin(ssidToSet, passwordToSet);
    } else if (c == cmdWdSelfTest) {
      Serial.println("starting self-test (infinite loop, should reboot soon)");
      while (true)
        ;
    } else if (c == cmdHelp) {
      Serial.println("Available commands:");
      Serial.println();
      Serial.println(cli.toString());
    }
  }

  if (cli.errored()) {
    Serial.print("ERROR: ");
    Serial.println(cli.getError().toString());
  }
}
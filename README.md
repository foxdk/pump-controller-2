# ESP32 Arduino Home Hot Water Recirculation Pump Controller

## User Guide

### What This Project Provides

The intent of this project, together with appropriate plumbing and hardware, is to speed delivery of hot water to parts of a home that are faw away from the water heater. After installation, anyone in the home turning on a hot water tap will get hot water more quickly and with less wasted water.

### Features

* Provides hot water more quickly and with less wasted water
* Hot water provided on demand, which helps save gas or electricity
* Ability to see hot water flow and pump data via a web browser connected to the home Wi-Fi
* Ability to set home Wi-Fi credentials using a serial port monitor with USB-C cable
* Does not require temperature probes
* Protects the pump and water heater from overheating (see below)


#### Pump and Water Heater Protections

* Protects pump and water heater from overheating by ensuring that pump runs no more than a single 90-second interval in each 20-minute time window
* Employs an ESP32 hardware watchdog timer to ensure that any long software delay (such as an infinite loop) will reboot the controller
* Provides a watchdog self-test from a terminal-based command-line utility to show that the watchdog timer is working

### Quick Start Guide

#### Adjust Hard-Coded Configuration

This project contains hard-code configuration values that depend on your own plumbing topology. See the values in pc-constants.h, study their use in the code, and change them to fit your own plumbing topology.

#### Setting Wi-Fi Credentials

1. Turn the controller off (so that the pump won't run)
2. Disconnect the ESP32-C3 from the dedicated USB-C power supply
3. Connect the ESP32-C3 USB-C port to a computer running an Arduino IDE
4. Connect the Arduino IDE to the correct serial port (usually COM3 or COM4 for me)
5. Use the "setupwifi" command to provide Wi-Fi credentials
```
setupwifi my-ssid my-password
```
6. The credentials will be saved until the next time they are changed
7. Unplug the ESP32-C3 from the computer and plug it back in to its dedicated power supply
8. Turn the controller back on

## Circuit and Plumbing Requirements

This project is Arduino code for an ESP32-C3-based controller for a hot water recirculation pump. I had a plumber install the hot water recirc loop and used a Taco 0015-SF2-21FC for the recirc pump.

I used the Seeed Studio XIAO ESP32C3, which I've been really pleased with. For the LED output pins I used a 220-ohm resistor in series with the LED between the output pin and ground. To control the pump I used a Digital Loggers IOT relay, which I've also been really pleased with. For the digital flow meter I used something similar to a XAXAX 262025231.

## Software Dev Environment

Initially, I tried to use VSCode with the PlatformIO extension, but eventually discovered that Espressif does not support the latest ESP32 Arduino libraries when using PlatformIO. So, I settled on the Arduino IDE with the XIAO_ESP32C3 board from the Espressif ESP32 board set.

## License Agreement and Disclaimer

Please see the LICENSE file for the license agreement and disclaimer that applies to this project.

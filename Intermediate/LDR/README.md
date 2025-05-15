# Light Sensor with LED Indicator

## Overview

This project uses an LDR (Light Dependent Resistor) sensor with an ESP32 to detect light levels and turn an LED on or off based on brightness.

## Features

1. Uses an LDR sensor to detect ambient light

2. Automatically turns an LED on when it's dark and off when it's bright

3. Simple implementation with minimal components

## Requirements

1. ESP32 development board

2. LDR sensor

3. LED

4. Resistor (if required for the LED)

5. Connecting wires and a breadboard

## How It Works

* The LDR sensor detects the ambient light level.

* If the light level falls below a predefined threshold (ANALOG_THRESHOLD = 300), the LED turns on.

* If the light level is above the threshold, the LED turns off.

* The ESP32 continuously reads the light sensor and updates the LED state accordingly.

## Setup Instructions

Connect the LDR sensor to the ESP32:

1. One leg of the LDR to 3.3V

2. The other leg to an analog input pin (LDR_PIN) with a pull-down resistor to GND

3. Connect the LED to the ESP32 through an appropriate resistor.

4. Upload the provided code to the ESP32 board.

5. Observe the LED turning on and off based on the ambient light level.
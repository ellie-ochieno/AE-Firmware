
# ESP32 Button-Controlled LED Toggle


## Overview

This project demonstrates how to toggle an LED using a button press with an ESP32. The button acts as a switch, and every time it is pressed, the LED state changes (ON to OFF or OFF to ON).

## Features

- Uses an external push button to toggle an LED

- Implements an internal pull-up resistor to stabilize the button input

- Debounces button presses for reliable operation

- Prints LED toggle status to the Serial Monitor

## Requirements

- ESP32 development board

- Push button

- LED

- 220Ω resistor (if required for the LED)

- 10kΩ resistor (optional, if using an external pull-up instead of INPUT_PULLUP)

- Connecting wires and a breadboard

## How It Works

* The button is connected to pin 2 and configured as an input with an internal pull-up resistor.

* The LED is connected to pin 4 and configured as an output.

* When the button is pressed, it toggles the state of the LED.

* The Serial Monitor displays a message every time the LED state changes.

## Setup Instructions

1. Connect the components as per the circuit diagram:

2. One side of the button to ground (GND)

3. The other side to pin 2 (configured with INPUT_PULLUP)

4. LED connected to pin 4 with a resistor (if required)

5. Upload the provided code to the ESP32 board.

6. Open the Serial Monitor at 9600 baud to observe the LED state changes.

7. Press the button to toggle the LED on and off.


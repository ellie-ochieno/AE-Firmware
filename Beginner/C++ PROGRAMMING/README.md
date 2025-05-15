
# Arduino LED Blinking Programs

## Overview
This repository contains three different Arduino programs demonstrating various LED blinking patterns using the built-in LED or an external LED connected to a digital pin.

## Requirements
* Esp32 (Angaza Elimu board)
* LED (if using an external one)
* Resistor (220Ω recommended, if using an external LED)
* USB cable for uploading the code
* Arduino IDE

## Code Descriptions
1. Basic LED Toggle

* Uses pin 13 for the onboard LED.
* Reads the current LED state and toggles it every second.

2. Blink LED for 5 Cycles with a Pause

* Uses pin 13 for the onboard LED.
* Blinks the LED on and off for 5 cycles with 500ms intervals.
* After 5 cycles, waits for 3 seconds before repeating.

3. Multiple Blink Sequences and Exit

* Uses pin 9 for the LED.
* Blinks the LED 5 times, waits 1 second, blinks 3 times, waits 1 second, blinks 2 times, then exits the loop.

## How to Use
1. Open the Arduino IDE.
2. Copy and paste the desired program into the IDE.
3. Connect your Arduino board to your computer.
4. Select the correct board and port in the IDE settings.
5. Click the "Upload" button to flash the program onto your Arduino.
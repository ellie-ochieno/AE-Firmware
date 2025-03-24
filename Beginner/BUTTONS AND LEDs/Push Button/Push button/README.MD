
# Arduino LED Control Programs

## Overview
This repository contains various Arduino programs demonstrating LED control using the built-in LED or an external LED connected to a digital pin. These programs include simple blinking patterns and button-controlled LED behavior.

## Requirements
* Arduino board (e.g., Arduino Uno)
* LED (if using an external one)
* Resistor (220Ω recommended, if using an external LED)
* Push button (for button-controlled program)
* USB cable for uploading the code
* Arduino IDE

## Code Descriptions
- Uses pin 9 for the LED and pin 4 (GPIO19) for the button.
- The button is configured with an internal pull-up resistor.
- When the button is pressed (pulled LOW), the LED turns on.
- When the button is not pressed (HIGH), the LED remains off.

## How to Use
1. Open the Arduino IDE.
2. Copy and paste the desired program into the IDE.
3. Connect your Arduino board to your computer.
4. Select the correct board and port in the IDE settings.
5. Click the "Upload" button to flash the program onto your Arduino.
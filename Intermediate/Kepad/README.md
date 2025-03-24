# Keypad-Controlled Servo Lock

## Overview

This project implements a simple security system using an ESP32, a 4x4 keypad, and a servo motor. The user must enter a correct password to unlock the door.

## Features

1. Uses a 4x4 keypad for input

2. Controls a servo motor to simulate door locking/unlocking

3. Password-protected access

4. Serial monitor feedback for debugging

## Requirements

1. ESP32 development board

2. 4x4 Keypad

3. Servo motor

4. Connecting wires and a breadboard

## How It Works

1. The user enters a password using the 4x4 keypad.

2. If the entered password matches the predefined password, the servo unlocks the door.

3. After 5 seconds, the servo closes the door again.

4. If an incorrect password is entered, an error message is displayed on the Serial Monitor.

## Setup Instructions

1. Connect the keypad to the ESP32:

* Row pins to ESP32 GPIOs 1, 10, 9, 11

* Column pins to ESP32 GPIOs 12, 14, 21, 34

* Connect the servo motor to pin 13 on the ESP32.

* Upload the provided code to the ESP32 board.

* Open the Serial Monitor at 9600 baud to observe password entry and door status.

* Enter the password (1234 by default) using the keypad.

* If correct, the door opens and automatically closes after 5 seconds.
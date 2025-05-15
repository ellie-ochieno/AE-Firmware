# PIR Motion Sensor

## Overview

This project uses a PIR motion sensor with an ESP32 to detect movement and control an LED.

## Features

1. Detects motion using a PIR sensor

2. Turns an LED ON when motion is detected

3. Turns the LED OFF when motion stops

4. Displays motion status on the Serial Monitor

## Requirements

1. ESP32 development board

2. PIR motion sensor

3. LED

4. Connecting wires and a breadboard

## How It Works

1. The PIR sensor detects movement and outputs a HIGH signal.

2. The ESP32 reads this signal and turns the LED ON.

3. When motion stops, the LED turns OFF.

4. Motion status is printed on the Serial Monitor.

## Setup Instructions

1. Connect the PIR sensor to pin 39.

2. Connect the LED to pin 9.

3. Upload the provided code to the ESP32.

4. Monitor motion detection via Serial Monitor.
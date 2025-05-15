# Gas Sensor with Alarm

## Overview

This project uses a gas sensor with an ESP32 to detect harmful gas levels and trigger an LED and buzzer alarm when a threshold is exceeded.

## Features

1. Monitors gas levels using an analog gas sensor

2. Activates an LED and buzzer when gas levels exceed a certain threshold

3. Provides real-time gas level readings on the Serial Monitor

## Requirements

1. ESP32 development board

2. Gas sensor (e.g., MQ-2, MQ-5, or similar)

3. LED

4. Buzzer

5. Connecting wires and a breadboard

## How It Works

1. The ESP32 reads gas levels from the sensor connected to pin 20.

2. If the gas level exceeds 1000, an LED and buzzer are activated.

3. The buzzer produces a tone using PWM (Pulse Width Modulation).

4. Gas level readings are continuously displayed on the Serial Monitor.

5. The system checks gas levels every 100 milliseconds.

## Setup Instructions

Connect the gas sensor:

1. Analog output to ESP32 pin 20

2. Power and ground connections as per sensor specifications

3. Connect the LED to pin 9 and the buzzer to pin 15.

4. Upload the provided code to the ESP32 board.

5. Open the Serial Monitor at 115200 baud to observe gas level readings.

6. If gas levels exceed the threshold, the LED and buzzer will activate.
# Water Level Detection System

## Overview

This project uses an ESP32 to detect water levels and trigger a buzzer alarm when a set threshold is exceeded.

## Features

1. Monitors water levels using an analog sensor

2. Activates a buzzer when the water level surpasses a defined threshold

3. Displays real-time water level readings on the Serial Monitor

Requirements

ESP32 development board

Water level sensor

Buzzer

Connecting wires and a breadboard

## How It Works

1. The ESP32 reads the water level from the sensor connected to pin 32.

2. If the water level exceeds 450, the buzzer is activated.

3. The buzzer remains on for 5 seconds before turning off.

4. Water level readings are displayed on the Serial Monitor every 5 seconds.

# ESP32 LED Control Web Server

## Overview

This project creates a simple ESP32 web server to control multiple LEDs remotely using a web interface.


## Setup Instructions

1. Install the required libraries in the Arduino IDE.

2. Update the WiFi credentials in the code:
```
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```
3. Upload the code to the ESP32 board.

4. Open the Serial Monitor (115200 baud rate) to view the IP address assigned to the ESP32.

5. Access the web server by entering the ESP32's IP address in a web browser.

## Features

1. Allows remote control of Red, Blue, and Green LEDs via a web interface.

2. Hosts a web server on the ESP32.

3. Provides ON/OFF buttons for each LED.

## How It Works

1. The ESP32 hosts an HTML page with buttons for each LED.

2. Clicking a button sends an HTTP GET request to toggle the LED state.

3. The ESP32 reads the request and controls the corresponding GPIO.
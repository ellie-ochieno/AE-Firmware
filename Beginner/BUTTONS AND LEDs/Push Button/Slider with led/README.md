
# ESP32 WebSocket RGB LED Control

## Overview
This project demonstrates how to control an RGB LED using an ESP32 microcontroller via a web interface. Users can adjust the red, green, and blue intensity values using a web-based slider interface, which communicates with the ESP32 over WebSockets

## Requirements
* Esp32 
* WiFi network for ESP32 to connect
* 3x 220Ω resistors (if required for your LED setup)
* Libraries
* USB cable for uploading the code
* Arduino IDE
## How It Works

- The ESP32 connects to a WiFi network using the provided SSID and password.

- A web server is hosted on the ESP32, serving an HTML page with sliders for controlling the RGB LED.

- Users adjust the color values using sliders, which send JSON data over a WebSocket connection.

- The ESP32 parses the received JSON and updates the LED brightness using PWM (Pulse Width Modulation).
##  Setup Instructions

- Install the required libraries in the Arduino IDE.
- Update the ssid and password variables with your WiFi credentials.
- Upload the code to your ESP32 board.
- Open the Serial Monitor to find the assigned IP address.

Connect to the web interface using a browser and control the LED.

## How to Use
1. Open the Arduino IDE.
2. Copy and paste the desired program into the IDE.
3. Connect your Arduino board to your computer.
4. Select the correct board and port in the IDE settings.
5. Click the "Upload" button to flash the program onto your Arduino.
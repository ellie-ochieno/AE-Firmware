# ESP32 Servo Motor Web Server

## Overview

This project sets up an ESP32 web server that allows users to control a servo motor using a web-based slider.



## Setup Instructions

1. Install the required libraries in the Arduino IDE.

2. Update the WiFi credentials in the code:
```
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```
3. Upload the code to the ESP32 board.

4. Open the Serial Monitor (9600 baud rate) to view the IP address assigned to the ESP32.

5. Access the web server by entering the ESP32's IP address in a web browser.

## Features

1. Controls a servo motor using a web-based slider.

2. Hosts a web server on the ESP32.

3. Updates servo position in real time.

## How It Works

1. The ESP32 hosts an HTML page with a slider input.

2. The slider sends values via HTTP GET requests to the ESP32.

3. The ESP32 reads the values and moves the servo accordingly.
# ESP32 DHT Sensor Web Server

## Overview

This project sets up an ESP32 web server that reads temperature and humidity data from a DHT sensor and displays it on a web page. The data is updated every 10 seconds using JavaScript.



## Setup Instructions

1. Install the required libraries in the Arduino IDE.

Update the WiFi credentials in the code:
```
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```
2. Upload the code to the ESP32 board.

3. Open the Serial Monitor (115200 baud rate) to view the IP address assigned to the ESP32.

4. Access the web server by entering the ESP32's IP address in a web browser.

## Features

1. Reads temperature and humidity data from the DHT sensor.

2. Hosts a web server on the ESP32.

3. Displays real-time sensor data on a web page.

4. Auto-refreshes the data every 10 seconds.

## API Endpoints

    / - Returns the main HTML page.

    /temperature - Returns the current temperature reading.

    /humidity - Returns the current humidity reading.
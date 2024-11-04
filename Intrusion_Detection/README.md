# Intrusion Detection Control System
## Overview

The Intrusion Detection Control System is designed to detect motion using a PIR (Passive Infrared) sensor. When motion is detected, the system sends a notification to a server for further action. This project is part of the Angaza Elimu - Smart Home Project V2.0.
## Features

* Detects motion using a PIR sensor.
* Sends the state of the PIR sensor to a server via HTTPS.
* Provides real-time updates on motion detection status.

Components Required

        1. ESP32 
        2. PIR Sensor
        3. LED (for onboard indication)
        4. Buzzer (optional for alerts)
        5. Connecting wires

## Pin Configuration

|    PinName       |               PinNumber                                                      |
| ----------------- | ------------------------------------------------------------------ |
| PIR_PIN |39 |
| ONBOARD_LED_PIN | 13|

## Libraries Used

    WiFi.h: To manage WiFi connections.
    HTTPClient.h: For HTTP requests.
    Arduino_JSON.h: For parsing JSON data.
    WiFiClientSecure.h: For secure HTTPS connections.

## Setup Instructions

1. Install Required Libraries: Make sure to install the required libraries in your Arduino IDE.

2. Configure WiFi Credentials: In the secrets.h file, set your WiFi SSID and Password:

   
```
const char* SSID = "your_SSID";
const char* PASSWORD = "your_PASSWORD";
```

3. Set API Key: In the secrets.h file, set your API key:
```
const char* API_KEY = "your_API_KEY";
```
4. Connect the Components:
* Connect the PIR sensor to the designated pin (PIR_PIN).
* Connect the onboard LED to ONBOARD_LED_PIN for visual feedback.

5. Upload the Code: Upload the provided code to your ESP32 using the Arduino IDE.

## Usage

After uploading the code and powering on the ESP32, it will attempt to connect to the specified WiFi network.

The onboard LED will blink while attempting to connect and turn off once connected.

The system will continuously monitor the PIR sensor. If motion is detected, it will send a POST request to the specified server with the status of the PIR sensor.

You can monitor the serial output for the connection status and motion detection logs.

## Code Explanation
Setup Function

- Initializes the serial communication and sets the pin modes.
- Connects to the specified WiFi network and indicates connection status through the onboard LED.

Loop Function

- Continuously checks the state of the PIR sensor.
- If motion is detected, it sends the updated status to the server.
    Uses a delay to avoid overwhelming the server with requests.

HTTP Request Function

- Prepares and sends an HTTP POST request with the PIR sensor status to the server.
-  Handles the server's response and prints it to the serial monitor.


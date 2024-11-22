## Smoke Detection Handler 

This project uses an ESP32 microcontroller and an MQ2 Smoke Sensor to detect and respond to high smoke levels. The system:

  - Continuously monitors smoke levels.
  - Sends the data to a remote server via HTTPS.
  - Compares the smoke levels against a threshold received from the server.
  - Activates a buzzer when the detected level exceeds the threshold.


## Features

  **Real-Time Smoke Monitoring:** Continuously reads smoke levels using the MQ2 sensor.

  **Remote Threshold Configuration:** Fetches smoke threshold dynamically from a remote server.

  **Alerts and Alarms:** Activates a buzzer if smoke exceeds the set threshold.

  **WiFi Connectivity:** Enables secure communication with a server.

  **Visual Feedback:** Uses an onboard LED to indicate connection status.


## Hardware Components:

1. ESP32 Development Board
2. MQ2 Smoke Sensor
3. Buzzer

## Software:

1. Arduino IDE
2. Libraries:
  - WiFi.h - WiFi functionality.
  - HTTPClient.h - Handling HTTP requests.
  - WiFiClientSecure.h - Secure HTTPS communication.
  - Arduino_JSON.h - JSON parsing.



## Configuration Files:

  secrets.h: Define your WiFi credentials and API key:

    #define SSID "your_wifi_ssid"
    #define PASSWORD "your_wifi_password"
    #define API_KEY "your_api_key"

pin_configurations.h: Define the GPIO pin mappings:

    #define MQ2_PIN 34  // Analog input pin for MQ2 sensor
    #define BUZZER_PIN 25 // GPIO for the buzzer
    #define ONBOARD_LED_PIN 2 // GPIO for onboard LED

## How It Works
System Workflow:

1. Startup:
  - ESP32 connects to a specified WiFi network.
  - The onboard LED flashes during connection and stabilizes when connected.

2. Smoke Monitoring:
  - The MQ2 sensor continuously reads analog smoke levels.
  - Values are logged to the serial monitor.

3. Server Communication:
  - Sends sensor readings to the remote server via a POST request.
  - Retrieves the threshold value from the server's response.

4. Threshold Check:
  - If the smoke value exceeds the threshold:
    - Activates the buzzer alarm using PWM tones.

5. Loop Control:
  - Delays for 3 seconds between each cycle.

## Key Functions
WiFi Initialization:

The ESP32 connects to a WiFi network, and the onboard LED provides connection feedback.
HTTPS Communication:

  - The ESP32 uses a secure client with SSL/TLS for communication.
  - Sends sensor data and retrieves threshold values using httpGETRequest.

Smoke Detection Logic:

  - Reads analog data from the MQ2 sensor.
  - Activates the buzzer if the smoke level exceeds the threshold.
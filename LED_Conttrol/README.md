# SmartLights Control System

This project demonstrates a smart lighting system using an ESP32 microcontroller to control an LED remotely via a secure HTTP connection. The LED can be toggled ON/OFF, blink at a platform-defined interval, or follow a scheduled ON/OFF timer, all managed from a central web platform.

---

## Features

- **Remote Control:** Toggle the LED ON/OFF via API commands.
- **Blink Functionality:** Configure the LED to blink at intervals defined by the web platform.
- **Scheduled Operations:** The system can schedule LED operations using platform timers.

---

## Getting Started

### Prerequisites

1. **Hardware:**
   - ESP32 development board
   - Onboard LED or external LED with a suitable resistor

2. **Software:**
   - [Arduino IDE](https://www.arduino.cc/en/software) with ESP32 board support installed.
   - Platform credentials (`secrets.h`) and pin definitions (`pin_configurations.h`).

### Libraries Used

- `WiFi.h`: For Wi-Fi connectivity.
- `HTTPClient.h`: To handle HTTP requests.
- `WebServer.h`: For web server functionality.
- `ESPmDNS.h`: For mDNS support.
- `WiFiClientSecure.h`: To enable HTTPS communication.
- `Arduino_JSON.h`: For JSON parsing.

---

## Configuration

1. **Add Platform Credentials:**
   Create a `secrets.h` file with the following content:

   ```cpp
   #define SSID "your_wifi_ssid"
   #define PASSWORD "your_wifi_password"
   #define apiKeyValue "your_api_key"

2. Pin Definitions: Define the LED pin in pin_configurations.h
  
  ````cpp
  #define ONBOARD_LED_PIN 2  // Change according to your board's LED pin
 ````

## How it Works

1. Wi-Fi Initialization: The ESP32 connects to the Wi-Fi network and establishes an HTTPS connection with the web platform.
2. Server Communication:
  - Periodically sends GET requests to fetch the LED control commands.
  - Reads platform responses containing:
      - led_control_state (1 for ON, 0 for OFF)
      - led_blink_interval (in milliseconds)
      - led_blink_intensity (for future use).
3. LED Control:
  - Executes commands received from the platform:
      - Turns the LED ON or OFF.
      - Implements blinking functionality based on the interval provided.
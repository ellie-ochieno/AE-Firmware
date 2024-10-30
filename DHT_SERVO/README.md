
# Temperature and Humidity Monitoring System with Fan Control

This project is designed to monitor room temperature and humidity levels using a DHT sensor, send data to an IoT platform, and control a fan based on temperature thresholds set via the platform. This solution can be useful in smart home or office environments for climate control and monitoring.

## Features
* Measures room temperature and humidity using a DHT sensor.
* Sends recorded data to a cloud platform for remote monitoring.
* Retrieves and applies temperature thresholds set from the platform to control a fan.
* Displays real-time temperature and humidity on an OLED screen.

## Project Overview
- Platform: Angaza Elimu - SH Project V2.0/023
- Target Audience: IoT and smart home enthusiasts
- Modules Used:
    * DHT22 sensor for temperature and humidity
    * SSD1306 OLED display
    * WiFi (ESP32 module)
    * Fan controlled via digital pins

## Hardware Requirements

    1. ESP32 Microcontroller
    2. DHT22 Temperature & Humidity Sensor
    3. SSD1306 OLED Display (128x64 pixels)
    4. Fan 
    5. WiFi connection
    6. Connection wires
    7. Power supply

## Pin Configurations

| PinName            | PinNumber                                                                |
| ----------------- | ------------------------------------------------------------------ |
| PUSH_BUTTON_PIN |  4, 40|
| WATER_LEVEL_PIN| 5|
| LDR_PIN | 6|
| DHT_PIN | 7|
| ONBOARD_LED_PIN | 13|
| BUZZER_PIN | 15|
| INA_PIN| 38|
| INB_PIN |35 |
| MQ2_PIN | 34|
| PIR_PIN |39 |
| SERVO_PIN | 20|
| RED_PIN |16 |
| BLUE_PIN | 19|
| GREEN_PIN | 33|
| LRCL_PIN | 17|
| BCLK_PIN | 18|
| DOUT_PIN | 8|
| I2C_SCL | 2 (SCL)|
| I2C_SDA |42 (SDA) |

## Libraries

Ensure the following libraries are installed in your Arduino IDE:

    DHT Sensor Library
    WiFi.h (for ESP32 WiFi module)
    HTTPClient
    Arduino_JSON (for JSON parsing)
    Adafruit_GFX
    Adafruit_SSD1306

## Setup and Configuration

### Connect Hardware:
        - DHT Sensor: Connect the data pin to the specified GPIO pin on the ESP32 (defined as DHT_PIN in pin_configurations.h).
        - OLED Display: Connect SDA and SCL pins as defined (I2C_SDA and I2C_SCL).
        - Fan: Connect to INA_PIN and INB_PIN pins for controlling the ON/OFF states.

### Platform Connection:
        - Configure WiFi credentials by adding your SSID and PASSWORD in the secrets.h file.
        - Update API_KEY and server_url in secrets.h for server requests.

### Code Adjustments:
        - temp_threshold and humid_threshold values are fetched from the server.
        - To customize thresholds, modify the settings on the connected platform.

### Upload Code:
        - Connect the ESP32 to your computer and upload the code.
        - Monitor serial output to verify successful connections and data logs.

## Usage
1. Boot up the system:
* Upon powering, the ESP32 will attempt to connect to WiFi, indicated by a blinking onboard LED.
* Once connected, it will begin reading temperature and humidity data.

2. Data Handling:
*    Temperature and humidity values are continuously read from the DHT sensor.
* Data is sent to the specified platform server every few seconds.
* If the temperature exceeds the threshold set on the platform, the fan is triggered to cool the area.

3. Display Output:
* Temperature and humidity readings are displayed on the OLED screen in real-time.

4. Serial Monitoring:

* View logs on the Serial Monitor to check HTTP requests and device status.

## Troubleshooting

* WiFi Connectivity: Ensure WiFi credentials are correct and the network is stable.
* Sensor Read Failures: If DHT sensor readings fail, check the sensor wiring.
* Fan Control Issues: Confirm that INA and INB pins are correctly wired to the fan control circuit.
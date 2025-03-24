# DHT22 Sensor with OLED Display and Fan Control

### Overview

This project uses an ESP32 to read temperature and humidity from a DHT22 sensor and display the values on an OLED screen. Additionally, it controls a fan (or motor) based on the temperature.

## Features

1. Reads temperature and humidity using a DHT22 sensor

2. Displays sensor readings on an SSD1306 OLED screen

3. Controls a fan or motor when the temperature exceeds 25°C

## Requirements

1. ESP32 development board

2. DHT22 sensor

3. SSD1306 OLED display (I2C)

4. Fan 

5. Connecting wires and a breadboard

## How It Works

- The ESP32 reads temperature and humidity data from the DHT22 sensor.

- The data is displayed on the OLED screen.

- If the temperature exceeds 25°C, the fan/motor is turned ON for 5 seconds.

- Otherwise, the fan remains OFF.

- Sensor readings update every 10 seconds.

## Setup Instructions

Connect the components:

    DHT22 data pin to the designated ESP32 pin (DHT_PIN)

    OLED display to ESP32 using I2C (SDA and SCL pins as defined in pin_configurations.h)

    Fan/motor connected to INA_PIN and INB_PIN

    Upload the provided code to the ESP32 board.

    Open the Serial Monitor at 115200 baud to observe sensor readings and fan control status.

    Observe the temperature and humidity values on the OLED screen.
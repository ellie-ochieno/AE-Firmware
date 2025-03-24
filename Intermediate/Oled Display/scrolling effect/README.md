# IoT Sensor Display with OLED and DHT Sensor

This project demonstrates how to interface an OLED display with a microcontroller using I2C communication. It also includes the setup for a DHT temperature and humidity sensor, though the sensor functionality is not yet implemented in this version.

## Features

1. Initializes and configures an SSD1306 OLED display

2. Uses custom I2C pins defined in pin_configurations.h

3. Displays a scrolling message: "Hello, from Angaza Elimu!"

4. Serial communication enabled for debugging

5. Implements scrolling effects in various directions on the OLED screen

## Libraries Used

Ensure you have installed the following libraries in the Arduino IDE before uploading the code:

    Adafruit_GFX (for graphics support)

    Adafruit_SSD1306 (for OLED display control)

    Wire (for I2C communication)

    DHT (for temperature and humidity sensor)

    Wiring



## Installation and Usage

1. Clone or download this repository.

2. Open the .ino file in Arduino IDE.

3. Install necessary libraries via Library Manager.

4. Define the correct I2C and sensor pins in pin_configurations.h.

5. Compile and upload the code to your microcontroller.

6. Open the Serial Monitor (115200 baud rate) to check for initialization messages.

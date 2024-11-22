## Smart Window Control System - README

This project is a Smart Window Control System that automates window opening and closing based on a user-defined timer and environmental rain conditions. The system leverages IoT technology to provide real-time monitoring and control, using an ESP32 microcontroller and a connected servo motor to operate the window.
Key Features

1. Timer-Based Control:
        The window state can be scheduled for specific opening and closing times using [Angaza Elimu Platform](https://smart-home-iot.angazaelimu.com/).

2. Rain State Control:
        A rain sensor detects the presence of rain, triggering the window to close automatically.

3. Remote Monitoring:
        The system communicates with a cloud platform to report the rain sensor state and receive updated control parameters.

4. Real-Time Feedback:
        Live water level data from the sensor is displayed on the Serial Monitor.

5. Secure Communication:
        HTTPS is used for secure data transmission with the [Angaza Elimu Platform](https://smart-home-iot.angazaelimu.com/)

## Components Used

  **ESP32 Microcontroller:** Handles sensor readings, WiFi communication, and servo control.

  **Rain Sensor:**
        Detects rainwater and provides an analog water level reading.

  **Servo Motor:**
        Controls the opening and closing of the window.

  **WiFi Module:**
        Integrated into ESP32 for connecting to the internet.

  **LED Indicator:**
        Blinks during WiFi connection and remains steady when connected.

## Library Dependencies

Ensure the following libraries are installed in your Arduino IDE:

  - WiFi.h: For WiFi connectivity.
  - HTTPClient.h: For HTTP communication.
  - Arduino_JSON.h: For JSON parsing.
  - ESP32_New_ISR_Servo.h: For non-blocking servo motor control.

## Code Functionality

1. Initialization:
  - The ESP32 initializes the servo, sets up the rain sensor, and establishes a WiFi connection.
  - Once connected, the ESP32 communicates with the [Angaza Elimu Platform](https://smart-home-iot.angazaelimu.com/) to synchronize parameters.

2. Loop Process:
  - The ESP32 reads the rain sensor’s water level value.
  - Sends the data to the [Angaza Elimu Platform](https://smart-home-iot.angazaelimu.com/) using an HTTPS POST request.
  - Decodes the server response to get:
    - Rain threshold
    - Rain sensor state
    - Window state
  - Depending on the data:
    - Closes the window if rain is detected or a timer is active.
    - Opens the window when conditions are favorable.

3. Servo Control:
  - A smooth transition is achieved by incrementally adjusting the servo motor position.


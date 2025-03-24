## ESP32 Web Server for Controlling GPIOs

# Overview

This project creates a simple web server using an ESP32 microcontroller. The web server allows users to control three GPIO pins remotely through a web interface. The pins correspond to red, blue, and green LEDs (or any other connected output devices).

## Features

1. Connects ESP32 to a WiFi network

2. Hosts a web server accessible via a local IP address

3. Provides a web interface with buttons to turn GPIOs ON and OFF

5. Displays the current state of each GPIO pin



## WiFi Connection

```
 WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
} 
```

This section connects the ESP32 to the WiFi network.

### Web Server Handling

The ESP32 hosts a web server that listens for HTTP requests. Based on the request URL, it turns the respective GPIO ON or OFF.

```
if (header.indexOf("GET /16/on") >= 0) {
    digitalWrite(Red_pin, HIGH);
} else if (header.indexOf("GET /16/off") >= 0) {
    digitalWrite(Red_pin, LOW);
}
```

This part checks if the client requests to turn on or off a GPIO and updates its state accordingly.

### Web Page Generation

The server sends an HTML page with buttons to control each GPIO.

```
client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
```
This generates buttons that send requests to control GPIOs when clicked.

## How to Use

1. Upload the code to the ESP32 using the Arduino IDE.

2. Open the Serial Monitor to find the assigned IP address.

3. Open a web browser and enter the IP address.

4. Use the web interface to control the LEDs.


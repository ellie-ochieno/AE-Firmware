
# Smart Home Firmware

  - Arduino C
  - Version 2.0
  - /023

## How to create a project on the Angaza Elimu Platform
Head over to the Angaza Elimu platform and register for an account. The registration page allows you to create a user profile for accessing the platform's features. [Angaza Elimu Platform](https://smart-home-iot.angazaelimu.com/)

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/Signup.png)


## Login 
Once registered, use your credentials to log in to the Angaza Elimu platform.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/login.png)

## Create Project

Once you’re logged in, you will be taken to the homepage. Click the Create Project button to create a new project. Clicking the Create Project button on the left side menu bar will also take you to the page for creating a new project. 

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/Create%20project.png)

In the Create Project page, you will need to create a name for your project and select the hardware for your project. For this project we will name it temperature and humidity. Also, give it a meaningful project description as done below and click on create.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/temp_project.png)

After Successful project creation you will get this popup.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/Successful%20project%20creation.png)

## Hardware components

Connect the 4 pin Female-Female jumper cable to the OLED screen. 
Connect the SDA pin of the OLED screen to the SDA pin of the ESP32, SCL pin to SCL pin, GND pin to GND pin and VCC pin  to 3.3V pin
Connect the 3 pin Female - Female jumper cable to the DHT22 sensor. 
Connect the OUT pin of the DHT22 sensor to pin 7 of the ESP32, VCC (+) pin to 3.3V pin, and the GND (-) pin to GND pin, as shown in the diagram below.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/DHT_components.png)

## Api Key
Inside your project control page, you will see the API key. This API key is what will allow your IoT kit to communicate with the IoT platform. 

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/apikey.png)

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/data-1.png)

## Data Collection
On the screen we can be able to see the data in real time. The table below will show the temperature,humidity and the time it was collected.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/aad044f2a2269e7618c54b3a0069805f034542a1/Images/data_numbers.png)

## Threshhold
The threshold can be automatically set. Users can be able to set various thresholds to achieve different functionalities.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/set-threshold.png)


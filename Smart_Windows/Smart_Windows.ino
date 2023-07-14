/*
 * SMART WINDOW CONTROL-------------------
 * Control window based on:
 *    -> Set timer(Opening/Closing time)
 *    -> Rain state
 * AMGAZA ELIMU - SH. V2.0/023
 * ----------------------------------------
 */
 //---------------------Support libraries and sensor parameters.
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include "ESP32_New_ISR_Servo.h"
#include "pin_configurations.h"
#include "secrets.h"
#define TIMER_INTERRUPT_DEBUG       0
#define ISR_SERVO_DEBUG             1
// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#define USE_ESP32_TIMER_NO          3
// Published values for SG90 servos; adjust if needed
#define MIN_MICROS      544
#define MAX_MICROS      2450
//----------------------------------------

//----------------------------------------Control variables.
int servoIndex1  = -1;
// Platform connection URL
const char* server_url="https://smart-home-iot.angazaelimu.com/api/rainsensor_state_update";
WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;
String payload;
String httpRequestData;
String rain_sensor_state; //water level controlled
String window_state;      //timer controlled
String rainsensor_data;
int threshold;
int position;
int httpResponseCode;
int rain_threshold;
int water_level;
int windowStateCurrent   = LOW;  // current state of window (LOW=open HIGH=closed)
//----------------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(500);

  //Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);
  servoIndex1 = ESP32_ISR_Servos.setupServo(SERVO_PIN, MIN_MICROS, MAX_MICROS);
  if (servoIndex1 != -1)
    Serial.println(F("Setup Servo1 OK"));
  else
    Serial.println(F("Setup Servo1 failed"));

  pinMode(WATER_LEVEL_PIN, INPUT);
  delay(100);

  // start wifi instance
  WiFi.begin(SSID,PASSWORD);

  pinMode(ONBOARD_LED_PIN,OUTPUT);     //--> On Board LED port Direction output
  digitalWrite(ONBOARD_LED_PIN, HIGH); //--> Turn off Led On Board

  // initialize wifi connection
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ONBOARD_LED_PIN, LOW);
    delay(250);
    digitalWrite(ONBOARD_LED_PIN, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ONBOARD_LED_PIN, LOW); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  delay(1000);

  // server initialization
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  Serial.println("");
  Serial.print("connecting...");
  delay(1000);
}

void loop() {
  // get sensor readings
  water_level = analogRead(WATER_LEVEL_PIN); // read the analog value from sensor
  Serial.println("The water level value: " + String(water_level));

  if(client){
    // set secure client
    client->setCACert(root_cacert);
    //create an HTTPClient instance

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){
      //  get server response
      rainsensor_data = httpGETRequest(server_url);

      // decode response
      JSONVar json_rainsensor_data = JSON.parse(rainsensor_data);

      // validate response type
      if (JSON.typeof(json_rainsensor_data) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      rain_threshold = (const int)(json_rainsensor_data["rain_threshold"]);
      rain_sensor_state = (const char*)(json_rainsensor_data["rain_sensor_state"]);
      window_state = (const char*)(json_rainsensor_data["window_state"]);
      Serial.println("\nRain set threshold: "+ String(rain_threshold) +"");
      Serial.println("\nRain state: "+ rain_sensor_state +"");
      Serial.println("\nWindow state: "+ window_state +"");

      // control smart window state based on rain and set time state
      if(window_state=="1" || rain_sensor_state=="1"){
        if (windowStateCurrent==LOW){ //if the window is open - CLOSE
          for (position = 0; position <= 180; position+=10){
            // goes from 0 degrees to 180 degrees
            // in steps of 10 degree
            Serial.print(F("Servo1 pos = ")); Serial.print(position);
            ESP32_ISR_Servos.setPosition(servoIndex1, position);

            // waits 30ms for the servo to reach the position
            delay(30);
          }
        }
        windowStateCurrent = HIGH;  //change the current window state

      }else{//if rain threshold is less than water level value/open time elapsed
        if (windowStateCurrent==HIGH ){ //if the window is closed - OPEN
          for (position = 180; position >=0; position-=10){
            // goes from 0 degrees to 180 degrees
            // in steps of 10 degree
            Serial.print(F("Servo1 pos = ")); Serial.print(position);
            ESP32_ISR_Servos.setPosition(servoIndex1, position);

            // waits 30ms for the servo to reach the position
            delay(30);
            }
        }
        windowStateCurrent = LOW;
      }
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  delay(3000);
}

//  GET request function
String httpGETRequest(const char* serverName) {

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  //----------------------------------------GET HTTP Request
  // Prepare POST request data
  httpRequestData = "rain_sensor_API_KEY=" + String(API_KEY) + "&water_level=" + String(water_level) + "";
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  // Send POST request
  httpResponseCode = https.POST(httpRequestData);
  Serial.println("");
  Serial.print("Data request: ");
  Serial.print(httpRequestData);
  Serial.println("");
  //----------------------------------------

  //----------------------------------------GET HTTP Request
  if (httpResponseCode == 200) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();
  }
  else {                      //error if no GET data
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  https.end();

  return payload;
  //----------------------------------------
}

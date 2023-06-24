/*
 * This code  detects of there is water closes the windows
 */
 #include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include "ESP32_New_ISR_Servo.h"
#include "secrets.h"
#include "pin_configurations.h"

#define TIMER_INTERRUPT_DEBUG  0
#define ISR_SERVO_DEBUG        1

// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#define USE_ESP32_TIMER_NO     3


// Published values for SG90 servos; adjust if needed
#define MIN_MICROS   544
#define MAX_MICROS   2450

int servoIndex1  = -1;

const char* server_url="https://smart-home-iot.angazaelimu.com/api/rainsensor_state_update";

WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;

int threshold;
int position;
String payload;
String httpRequestData;
int httpResponseCode;

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
 if(client){
    // set secure client via root cert
    client->setCACert(root_cacert);
    //create an HTTPClient instance

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){

      //Get the threshold value
      String rainsensor_data= httpGETRequest(server_url);
    
      JSONVar json_threshold=JSON.parse(rainsensor_data);
    
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(json_threshold) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      int rain_threshold=(const int)(json_threshold["rain_threshold"]);
      String rain_sensor_state=(const char*)(json_threshold["rain_sensor_state"]);
    
      Serial.println("\nRain threshold: "+ String(rain_threshold) +"\n");
    
      int water_level = analogRead(WATER_LEVEL_PIN); // read the analog value from sensor
    
      Serial.println("The water sensor value: " + String(water_level));
    
      // control smart window state based on rain and set time state
      if((water_level>rain_threshold) || rain_sensor_state=="1"){
    
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Prepare POST request data
        httpRequestData = "rain_sensor_API_KEY=" + String(API_KEY) + "&rain_status=1";
        httpResponseCode=https.POST(httpRequestData);
    
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        // close smart window
        if ((servoIndex1 != -1 )&&(position==180)){
          for (position = 0; position <= 180; position+=10){
            // goes from 0 degrees to 180 degrees
            // in steps of 10 degree
            Serial.print(F("Servo1 pos = ")); Serial.print(position);
            ESP32_ISR_Servos.setPosition(servoIndex1, position);
    
            // waits 30ms for the servo to reach the position
            delay(30);
          }
        }
      }else{
    
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpResponseCode=https.POST("rain_sensor_API_KEY=" + String(API_KEY) + "&rain_status=0");
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
    
        // open smart window
        if ((servoIndex1 != -1 )&&(position==180)){
          for (position = 180; position >=0; position-=10){
            // goes from 180 degrees to 0 degrees
            // in steps of 10 degree
            Serial.print(F("Servo1 pos = ")); Serial.print(position);
            ESP32_ISR_Servos.setPosition(servoIndex1, position);
    
            // waits 30ms for the servo to reach the position
            delay(30);
          }
        }
      }
      https.end();
      delay(1000);
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  } 
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }

}

String httpGETRequest(const char* serverName) {

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Prepare POST request data
  httpRequestData = "rain_sensor_API_KEY=" + String(API_KEY);

  // Send POST request
  httpResponseCode = https.POST(httpRequestData);


  //get the response
  if (httpResponseCode == 200) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload=https.getString();
    Serial.println(payload);

  }
  else {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  return payload;
}

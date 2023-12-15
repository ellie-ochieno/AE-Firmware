/*
* INTRUSION DETECTION CONTROL-------------------
* Detect motion using:
*  -> PIR sensor
* ANGAZA ELIMU - SH.PROJECT V2.0 / 023
* ----------------------------------------
*/
//---------------------Support libraries and sensor parameters.
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
//#include <ESP32Servo.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"
int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin
//----------------------------------------

//----------------------------------------Control variables.
const char* server_url="https://smart-home-iot.angazaelimu.com/api/pirsensor_state_update";
WiFiClientSecure *client = new WiFiClientSecure;
String pir_sensor_status;
String httpRequestData;
int httpResponseCode;
HTTPClient https;
String payload;
//----------------------------------------

void setup() {
Serial.begin(115200);
pinMode(PIR_PIN, INPUT);  // set ESP32 pin to input mode to read value from OUTPUT pin of sensor

// wifi network init
WiFi.begin(SSID,PASSWORD);

// pins functionality and init states definition
pinMode(ONBOARD_LED_PIN,OUTPUT);     //--> On Board LED port Direction output
digitalWrite(ONBOARD_LED_PIN, HIGH); //--> Turn off Led On Board

// wifi connect
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

  // pir sensor control
  pinStatePrevious = pinStateCurrent; // store old state
  pinStateCurrent = digitalRead(PIR_PIN);   // read new state

  if(client){
    // set secure client
    client->setCACert(root_cacert);

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){

      if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
        Serial.println("Motion detected!");
        pir_sensor_status = "1"; // set sensor state
      }else{
        Serial.println("No notion detected!");
        pir_sensor_status = "0"; // set sensor state
      }

      // get server response
      httpGETRequest(server_url);

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

  //----------------------------------------Prepare HTTP Request
  // Prepare POST request data
  httpRequestData = "pir_sensor_API_KEY=" + String(API_KEY) + "&pir_status=" + pir_sensor_status + "";
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);
  Serial.print("\nPost request data: ");
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

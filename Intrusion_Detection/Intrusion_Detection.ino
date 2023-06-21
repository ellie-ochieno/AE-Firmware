#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <ESP32Servo.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"

int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin

const char* URL="https://smart-home-iot.angazaelimu.com/api/pirsensor_state_update";

WiFiClientSecure client;
HTTPClient https;
String payload;
String httpRequestData;
int httpResponseCode;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);  // set ESP32 pin to input mode to read value from OUTPUT pin of sensor

  WiFi.begin(SSID,PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi ...");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {

    if(WiFi.status()== WL_CONNECTED){

      https.begin(client, URL);
      httpGETRequest(URL);

      pinStatePrevious = pinStateCurrent; // store old state
      pinStateCurrent = digitalRead(PIR_PIN);   // read new state

      if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
        Serial.println("Motion detected!");

        //sending the data to the site
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Prepare POST request data
        httpRequestData = "pir_sensor_API_KEY=" + String(API_KEY) + "&pir_status=1";
        httpResponseCode=https.POST(httpRequestData);

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

      }
      else
      if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
        Serial.println("Motion stopped!");
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpResponseCode=https.POST("pir_sensor_API_KEY=" + String(API_KEY) +"&pir_status=0");

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

      }

    }

    delay(10000);
}

void httpGETRequest(const char* serverName) {

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Prepare POST request data
  httpRequestData = "pir_sensor_API_KEY=" + String(API_KEY);

  // Send POST request
  httpResponseCode = https.POST(httpRequestData);

  //get the response
  if (httpResponseCode == 200) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload=https.getString();
    Serial.println(payload);
  }
  else {
    //error if no GET data
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }


}


#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"

#define TONE_PWM_CHANNEL 0

/*
 * Gas pin. Can be connected to either the digital pin or analog pin
 * For this sketch, connect to the analog pin
*/

const char* URL="https://smart-home-iot.angazaelimu.com/api/smokesensor_state_update";

WiFiClientSecure client;
HTTPClient https;
String httpRequestData;
int httpResponseCode;
String payload;

void setup() {

  Serial.begin(115200);
  WiFi.begin(SSID,PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi ...");
  }
  Serial.println("Connected to the WiFi network");

  pinMode(MQ2_PIN, INPUT);
  ledcAttachPin(BUZZER_PIN, TONE_PWM_CHANNEL);
}

void loop() {

  // Your Domain name with URL path or IP address with path
  https.begin(client, URL);

  //Get the threshold value
  String smokesensor_data= httpGETRequest(URL);

  JSONVar json_threshold=JSON.parse(smokesensor_data);

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(json_threshold) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  int smoke_threshold=int(json_threshold["smoke_threshold"]);
  Serial.println("\nSet smoke threshold value: " + String(smoke_threshold));

  int gasValue = analogRead(MQ2_PIN);
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpResponseCode=https.POST("smokeValue=" +String(gasValue)+ "");
  Serial.print("HTTP Response code is: ");
  Serial.println(httpResponseCode);

  if (gasValue > smoke_threshold) {
    // Plays the middle C scale
    ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
    delay(500);
    ledcWriteTone(TONE_PWM_CHANNEL, 800);
    delay(500);
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Prepare POST request data
    httpRequestData = "smoke_sensor_API_KEY=" + String(API_KEY) + "&smoke_sensor_val=" + String(gasValue) + "";
    httpResponseCode=https.POST(httpRequestData);
    Serial.print("HTTP Response code is: ");
    Serial.println(httpResponseCode);

  }else{
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Prepare POST request data
    httpRequestData = "smoke_sensor_API_KEY=" + String(API_KEY) + "&smoke_sensor_val=" + String(gasValue) + "";
    httpResponseCode=https.POST(httpRequestData);
    Serial.print("HTTP Response code is: ");
    Serial.println(httpResponseCode);
  }
  https.end();
  delay(10000);
}

String httpGETRequest(const char* serverName) {

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Prepare POST request data
  httpRequestData = "smoke_sensor_API_KEY=" + String(API_KEY)+ " ";
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);


  //get the response
  if (httpResponseCode ==  200) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();
  }
  else {
    //error if no GET data
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  return payload;

}

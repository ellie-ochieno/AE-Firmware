
//----------------------------------------Support libraries and sensor parameters.
//#include <ESP32Servo.h>
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
//----------------------------------------


//----------------------------------------Control variables.

const char* server_url = "https://smart-home-iot.angazaelimu.com/api/smokesensor_state_update";
String httpRequestData;
int httpResponseCode;
String payload;
int gasValue;
int smoke_threshold;
String smokesensor_data;
WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;
//----------------------------------------


void setup() {
  Serial.begin(115200);
  delay(10);
  // Connect to WiFi network
  Serial.print("\n\nConnecting to ");
  Serial.println(SSID);
  // starts the wifi network

  // ONBOARD LED sensor pin states initialization
  pinMode(ONBOARD_LED_PIN, OUTPUT);    //--> On Board LED port Direction output
  digitalWrite(ONBOARD_LED_PIN, HIGH); //--> Turn off Led On Board

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //----------------------------------------Switch OnBoard LED to Flashing mode when connecting to the wifi.
    digitalWrite(ONBOARD_LED_PIN, LOW);
    delay(250);
    digitalWrite(ONBOARD_LED_PIN, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ONBOARD_LED_PIN, LOW); //--> Turn off the OnBoard LED when it is connected to the wifi.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  Serial.println("");
  delay(1000);
  Serial.print("connecting...");
  pinMode(MQ2_PIN, INPUT);
  ledcAttachPin(BUZZER_PIN, TONE_PWM_CHANNEL);

}

void loop() {
  // get sensor reading
  gasValue = analogRead(MQ2_PIN);
  Serial.println("\nSmoke sensor value: " + String(gasValue) + "\n");
  
  if(client){
    // set secure client
    client->setCACert(root_cacert);
    //create an HTTPClient instance
  
    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){
    
      //Get the threshold value
      smokesensor_data = httpGETRequest(server_url);
      JSONVar json_threshold=JSON.parse(smokesensor_data);
  
      // received data validation
      if (JSON.typeof(json_threshold) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      // received data decode
      smoke_threshold = (const int)(json_threshold["smoke_threshold"]);
      Serial.println("\nSet smoke threshold value: " + String(smoke_threshold));
  
      // invoke event when sensor value surpasses set threshold
      if (gasValue > smoke_threshold) {
         // Plays the middle C scale
         ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
         delay(500);
         ledcWriteTone(TONE_PWM_CHANNEL, 800);
         delay(500);
      }
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  
  // looping control delay
  delay(3000);
}
//  GET request function
String httpGETRequest(const char* serverName) {

  // define payload header contten-type
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  //----------------------------------------GET HTTP Request
  // Prepare POST request data
  httpRequestData = "smoke_sensor_API_KEY=" + String(API_KEY) + "&smoke_sensor_val=" + String(gasValue) + "";
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);
  
  Serial.print("Server POST request: ");
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
    Serial.print("\nError code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  https.end();

  return payload;
  //----------------------------------------
}


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

const char* server_url="https://smart-home-iot.angazaelimu.com/api/smokesensor_state_update";

WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;
String httpRequestData;
int httpResponseCode;
String payload;

void setup() {

  Serial.begin(115200);
  WiFi.begin(SSID,PASSWORD);

  pinMode(ONBOARD_LED_PIN,OUTPUT);     //--> On Board LED port Direction output
  digitalWrite(ONBOARD_LED_PIN, HIGH); //--> Turn off Led On Board

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

  
  pinMode(MQ2_PIN, INPUT);
  ledcAttachPin(BUZZER_PIN, TONE_PWM_CHANNEL);
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
      String smokesensor_data= httpGETRequest(server_url);
    
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

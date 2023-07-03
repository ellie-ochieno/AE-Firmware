#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
//#include <ESP32Servo.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"

int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin

const char* server_url="https://smart-home-iot.angazaelimu.com/api/pirsensor_state_update";

WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;
String payload;
int httpResponseCode;
String httpRequestData;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);  // set ESP32 pin to input mode to read value from OUTPUT pin of sensor

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

}

void loop() {
 if(client){
    // set secure client via root cert
    client->setCACert(root_cacert);
    //create an HTTPClient instance

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){

      httpGETRequest(server_url);

      pinStatePrevious = pinStateCurrent; // store old state
      pinStateCurrent = digitalRead(PIR_PIN);   // read new state

      if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
        Serial.println("Motion detected!");

        //sending the data to the site
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Prepare POST request data
        httpRequestData = "pir_sensor_API_KEY=" + String(API_KEY) + "&pir_status=" + "1" + "";
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
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
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

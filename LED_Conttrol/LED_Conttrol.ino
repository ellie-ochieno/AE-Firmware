//----------------------------------------Support libraries and sensor parameters.
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
//#include "connection_auth.h"
#include "secrets.h"
#include "pin_configurations.h"
//----------------------------------------


//---------------------------------------0-Network credentials definition.
const char* server_url = "https://smart-home-iot.angazaelimu.com/api/led_state_control";
//----------------------------------------


//----------------------------------------Control variables.
String led_state  = "";
String payload    = "";
int led_blink_intensity;
int led_blink_interval;
String led_data;
String httpRequestData = "0";
int httpResponseCode = 0;
unsigned int payload_sz;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;

//----------------------------------------


void setup() {

  Serial.begin(115200);
  delay(10);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  pinMode(ONBOARD_LED_PIN, OUTPUT);    //--> On Board LED port Direction output
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
  //----------------------------------------

  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  Serial.println("");
  delay(1000);
  Serial.print("connecting...");

}

void loop() {
 if(client){
    // set secure client
    client->setCACert(test_root_ca);
    //create an HTTPClient instance

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){
      //  get server response
      led_data = httpGETRequest(server_url);

      // decode response
      JSONVar json_res_data = JSON.parse(led_data);

      // validate response type
      if (JSON.typeof(json_res_data) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      // decode actual led status data
      led_state = (const char*)(json_res_data["led_control_state"]);
      led_blink_interval = (const int)(json_res_data["led_blink_interval"]);
      led_blink_intensity = (const int)(json_res_data["led_blink_intensity"]);
      Serial.println("");
      Serial.println("LED blink interval: " + String(led_blink_interval));
      Serial.println("LED blink intensity: " + String(led_blink_intensity));

      if (led_state == "1") { //turn ON signal LED
        digitalWrite(ONBOARD_LED_PIN, HIGH);
        Serial.println("LED status: ON");
        //blinks LED based on the defined time interval
        if(led_blink_interval > 0){
          digitalWrite(ONBOARD_LED_PIN, HIGH);
          delay(led_blink_interval);
          digitalWrite(ONBOARD_LED_PIN, LOW);
          delay(led_blink_interval);
        }
      }
      if (led_state == "0") { //turn OFF signal LED
        digitalWrite(ONBOARD_LED_PIN, LOW);
        Serial.println("LED status: OFF");
      }
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }

  //if LED blinking control interval is NOT set
  if(led_blink_interval <= 0){
    delay(3000);
  }
}
//  GET request function
String httpGETRequest(const char* serverName) {

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");


  //----------------------------------------GET HTTP Request
  // Prepare POST request initialize
  httpRequestData = "ledctrl_API_KEY=" + String(apiKeyValue) + "";
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
    Serial.println("");
    Serial.print("Payload: ");
    Serial.print(payload);
    Serial.println("");
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

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"
#define R_channel 0
#define G_channel 1
#define B_channel 2
#define pwm_Frequency 5000 // pwm frequency
#define pwm_resolution 8 // 8 bit resolution


const char* server_url="https://smart-home-iot.angazaelimu.com/api/rgbled_state_control";

String payload;
WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;
String httpRequestData;
String json_data;
int httpResponseCode= 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(SSID,PASSWORD);

  ledcAttachPin(RED_PIN, R_channel);
  ledcAttachPin(GREEN_PIN, G_channel);
  ledcAttachPin(BLUE_PIN, B_channel);
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);
  
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
  
      String rgbled_data= httpGETRequest(server_url);
      Serial.println(rgbled_data);
  
      JSONVar reading=JSON.parse(rgbled_data);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(reading) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
  
      String ledValue=(const char*)(reading["toggle_state"]);
      Serial.println(ledValue);
      if (ledValue=="1"){
        RGB_Color(255,78,98);
        Serial.println("RGBLED ON");
      }else if(ledValue=="0"){
        RGB_Color(0,0,0);
        Serial.println("RGBLED OFF");
      }
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  } 
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }

  delay(100);

}
 void RGB_Color(int i, int j, int k)
 {

  ledcWrite(R_channel, i);

  ledcWrite(G_channel, j);

  ledcWrite(B_channel, k);

 }
//  GET request function
String httpGETRequest(const char* serverName) {

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");


  //----------------------------------------GET HTTP Request
  // Prepare POST request data
  httpRequestData = "rgbled_toggle_API_KEY=" + String(API_KEY);
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);
  //----------------------------------------

  //----------------------------------------GET HTTP Request
  if (httpResponseCode > 0) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();
    Serial.println(payload);
  }
  else {//error if no GET data
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  https.end();

  return payload;

}

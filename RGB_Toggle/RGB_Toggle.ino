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


const char* URL="https://smart-home-iot.angazaelimu.com/api/rgbled_state_control";

String payload;
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

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi ...");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {

    String rgbled_data= httpGETRequest(URL);
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

  WiFiClientSecure client;
  HTTPClient https;

  // Initialize http protocol
  https.begin(client, serverName);
  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");


  //----------------------------------------GET HTTP Request
  // Prepare POST request data
  String httpRequestData = "rgbled_toggle_API_KEY=" + String(API_KEY);
  // Send POST request
  int httpResponseCode = https.POST(httpRequestData);
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

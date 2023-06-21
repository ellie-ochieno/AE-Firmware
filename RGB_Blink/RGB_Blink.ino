#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
//#include <ESP32Servo.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"
#define R_channel 0
#define G_channel 1
#define B_channel 2
#define pwm_Frequency 5000 // pwm frequency
#define pwm_resolution 8 // 8 bit resolution

const char* URL="https://smart-home-iot.angazaelimu.com/api/rgbled_state_control";

WiFiClientSecure client;
HTTPClient https;
String payload;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //Setup the LED
  ledcAttachPin(RED_PIN, R_channel);
  ledcAttachPin(GREEN_PIN, G_channel);
  ledcAttachPin(BLUE_PIN, B_channel);
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);

  WiFi.begin(SSID,PASSWORD);

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi ...");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {
  // Your Domain name with URL path or IP address with path
  https.begin(client, URL);

  //Get the threshold value
  String result= httpGETRequest(URL);
  Serial.println("The payload is: " +result);

  //Get the threshold value
  String rgbled_data= httpGETRequest(URL);

  JSONVar json_interval=JSON.parse(rgbled_data);
  Serial.println("The jsonvar is: "+json_interval);

  int blink_interval=(const int)(json_interval["blink_interval"]);
  Serial.println("Blink interval: "+ String(blink_interval));

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(json_interval) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  RGB_Color(100,200,100);
  delay(blink_interval);
  RGB_Color(0,0,0);
  delay(blink_interval);
  https.end();

}

void RGB_Color(int red, int green, int blue) {
  ledcWrite(R_channel, red);
  ledcWrite(G_channel, green);
  ledcWrite(B_channel, blue);
}

//  GET request function
String httpGETRequest(const char* serverName) {

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Prepare POST request data
  String httpRequestData = "rgbled_blink_API_KEY" + String(API_KEY);

  // Send POST request
  int httpResponseCode = https.POST(httpRequestData);

  //get the response
  if (httpResponseCode == 200) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload=https.getString();
    Serial.println("The payload is: " +payload);

  }
  else {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  return payload;

}

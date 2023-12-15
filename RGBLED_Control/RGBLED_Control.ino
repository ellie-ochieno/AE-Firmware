/*
* RGBLED CONTROL-------------------
* Use RGBLED:
*  -> Control remotely
*     - Toggle, Blink and light by specific color from platform set values
* ANGAZA ELIMU - SH.PROJECT V2.0 / 023
* ----------------------------------------
*/
//----------------------------------------Support libraries and sensor parameters.
#include <WiFi.h>
#define R_channel 0
#define G_channel 1
#define B_channel 2
#include "secrets.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
//#include <ESP32Servo.h>
#include <WiFiClientSecure.h>
#include "pin_configurations.h"
#define pwm_Frequency 5000 // pwm frequency
#define pwm_resolution 8 // 8 bit resolution
//----------------------------------------

//----------------------------------------Control variables.
const char* server_url="https://smart-home-iot.angazaelimu.com/api/rgbled_state_control";
WiFiClientSecure *client = new WiFiClientSecure;
String httpRequestData;
String rgb_toggle_val;
int httpResponseCode;
String rgbled_data;
int blink_interval;
HTTPClient https;
String payload;
int greenValue;
int blueValue;
int redValue;
//----------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // led pins init and config
  ledcAttachPin(RED_PIN, R_channel);
  ledcAttachPin(GREEN_PIN, G_channel);
  ledcAttachPin(BLUE_PIN, B_channel);
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);

  // wifi network init
  WiFi.begin(SSID,PASSWORD);

  // pins function and states definition
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
  //------------------------------------------
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

        //Get the threshold value
        rgbled_data= httpGETRequest(server_url);

        JSONVar json_rgbled_data=JSON.parse(rgbled_data);
        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(json_rgbled_data) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        //decode response info
        blink_interval=(const int)(json_rgbled_data["blink_interval"]);
        rgb_toggle_val=(const char*)(json_rgbled_data["toggle_state"]);
        Serial.println("Blink interval: "+ String(blink_interval));
        Serial.println("RGBLED toggle state: " + rgb_toggle_val);
        // RGBLED color parameters
        redValue=(const int)(json_rgbled_data["rled_slider_val"]);
        Serial.println("\nRed led value: "+ String(redValue));
        greenValue=(const int)(json_rgbled_data["gled_slider_val"]);
        Serial.println("Green led value: "+ String(greenValue));
        blueValue=(const int)(json_rgbled_data["bled_slider_val"]);
        Serial.println("Blue led value: "+ String(blueValue)+"\n");

        if (rgb_toggle_val == "1") { //turn ON RGB LED
          // light RGB LED using color picker set values
          RGB_Color(redValue,greenValue,blueValue);
          //blinks RGB LED based on platform set time interval
          if(blink_interval > 0){
            RGB_Color(redValue,greenValue,blueValue);
            delay(blink_interval);
            RGB_Color(0,0,0);
            delay(blink_interval);
          }
          Serial.println("\nRGB LED status: ON\n");
        }
        if (rgb_toggle_val == "0") { //turn OFF RGB LED
          RGB_Color(0,0,0);
          Serial.println("\nRGB LED status: OFF\n");
        }
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  // if no blink interval is set
  if (blink_interval <= 0) {
    delay(3000);
  }
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

  //----------------------------------------Prepare HTTP Request
  httpRequestData = "rgbled_toggle_API_KEY=" + String(API_KEY) + "";
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);
  //----------------------------------------

  //----------------------------------------GET HTTP Request
  if (httpResponseCode > 0) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload=https.getString();
  }
  else {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  https.end();
  return payload;
  //----------------------------------------

}

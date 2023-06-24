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

const char* server_url="https://smart-home-iot.angazaelimu.com/api/rgbled_state_control";

WiFiClientSecure *client = new WiFiClientSecure;
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
      
        //Get the threshold value
        String result= httpGETRequest(server_url);
        Serial.println("The payload is: " +result);
      
        //Get the threshold value
        String rgbled_data= httpGETRequest(server_url);
      
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
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  } 
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  
  delay(3000);

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
  if (httpResponseCode > 0) { //initialize payload if GET data is available
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

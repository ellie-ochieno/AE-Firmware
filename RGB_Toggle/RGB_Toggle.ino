/*
* RGNLED CONTROL-------------------
* Use RGNLED:
*  -> Control remotely
*     - Toggle ON/OFF from the platform
* AMGAZA ELIMU - SH.PROJECT V2.0 / 023
* ----------------------------------------
*/
//----------------------------------------Support libraries and sensor parameters.
#include <WiFi.h>
#define R_channel 0
#define G_channel 1
#define B_channel 2
#include "secrets.h"
#define pwm_resolution 8 // 8 bit resolution
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#define pwm_Frequency 5000 // pwm frequency
#include <WiFiClientSecure.h>
#include "pin_configurations.h"
//----------------------------------------


//----------------------------------------Control variables.
const char* server_url="https://smart-home-iot.angazaelimu.com/api/rgbled_state_control";
WiFiClientSecure *client = new WiFiClientSecure;
int httpResponseCode= 0;
String httpRequestData;
String rgbled_data;
HTTPClient https;
String json_data;
String payload;
String rgb_toggle_val;
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

      // get server response
      rgbled_data= httpGETRequest(server_url);

      JSONVar json_rgbleddata=JSON.parse(rgbled_data);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(json_rgbleddata) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      // get toggle state
      rgb_toggle_val=(const char*)(json_rgbleddata["toggle_state"]);
      Serial.println("RGBLED toogel state: " + rgb_toggle_val);

      if (rgb_toggle_val=="1"){// toggle RGBLED ON
        RGB_Color(255,78,98);
        Serial.println("RGBLED ON");
      }else if(rgb_toggle_val=="0"){// toggle RGBLED OFF
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

  delay(1000);

}
//RGBLED control
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

    //----------------------------------------Prepare HTTP Request
    httpRequestData = "rgbled_toggle_API_KEY=" + String(API_KEY) + "";
    // Send POST request
    httpResponseCode = https.POST(httpRequestData);
    //----------------------------------------

    //----------------------------------------GET HTTP Request
    if (httpResponseCode > 0) {
      //initialize payload if GET data is available
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      payload = https.getString();
    }
    else {
      //error if no GET data
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    https.end();

    return payload;
    //----------------------------------------
}

/*
* TEMPERATURE&HUMIDITY MONITORING------------------
* USE DHT SENSOR:
*    -> Measure room temperature and humidity values
*    -> Send the recorded data to the platform
*    -> Set temperature threshold from the platform
*    -> Use set threshod and recorded data to control fan
* ANGAZA ELIMU - SH. PROJECT V2.0 / 023
* ----------------------------------------
*/
//---------------------Support libraries and sensor parameters.
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define DHT_SENSOR_TYPE DHT22
DHT dht_sensor(DHT_PIN, DHT_SENSOR_TYPE); // Create  DHT sensor object
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//----------------------------------------

//----------------------------------------Control variables.
const char* server_url="https://smart-home-iot.angazaelimu.com/api/dht_data_insert";
WiFiClientSecure *client = new WiFiClientSecure;
String httpRequestData;
int httpResponseCode;
int humid_threshold;
int temp_threshold;
HTTPClient https;
float humi  = 0;
float tempC = 0;
String payload;
//----------------------------------------

void setup() {
  Serial.begin(115200);
  // setting up custom I2C pins
  Wire.begin(I2C_SDA, I2C_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  // initialize display
  display.clearDisplay();
  display.setTextColor(WHITE);

  // pins functionality definition
  pinMode(INA_PIN, OUTPUT);
  pinMode(INB_PIN, OUTPUT);

  // wifi network init
  WiFi.begin(SSID,PASSWORD);

  // Signal LED pin function and state init
  pinMode(ONBOARD_LED_PIN,OUTPUT);     //--> On Board LED port Direction output
  digitalWrite(ONBOARD_LED_PIN, HIGH); //--> Turn off Led On Board

  // Connection init
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
  delay(10000);

  dht_sensor.begin(); // initialize the DHT sensor
  //----------------------------------------

  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  Serial.println("");
  Serial.print("connecting...");
  delay(10000);

}

void loop() {

  // read sensor parameter values
  humi  = dht_sensor.readHumidity(); // humidity
  tempC = dht_sensor.readTemperature(); // temperature in Celsius

  if(client){
    // set secure client via root cert
    client->setCACert(root_cacert);
    //create an HTTPClient instance

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){

      //Get the threshold value
      String dht_data= httpGETRequest(server_url);

      // decode response
      JSONVar json_threshold=JSON.parse(dht_data);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(json_threshold) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      // decode server set dht threshold
      temp_threshold=(const int)(json_threshold["temp_threshold"]);
      humid_threshold=(const int)(json_threshold["humid_threshold"]);

      // display threshod
      Serial.println("Set temperature threshold: " + String(temp_threshold) + "\n");

      // check whether the reading is successful or not
      if ( isnan(tempC) || isnan(humi)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }

      if (tempC>(temp_threshold)){// invoke fan into spin mode
          digitalWrite(INA_PIN,LOW);
          Serial.println("spinning");
          digitalWrite(INB_PIN,HIGH);
          delay(5000);
      }else{// stop fan
          digitalWrite(INA_PIN,LOW);
          Serial.println("stopped");
          digitalWrite(INB_PIN,LOW);
      }

      display.clearDisplay();  //clear display
      //print temperature
      display.setTextSize(1);
      display.setCursor(0,0);
      display.print("Temperature: ");
      display.setTextSize(2);
      display.setCursor(0,16);
      display.print(tempC);
      display.print(" ");
      display.setTextSize(1);
      display.cp437(true);
      display.write(167);
      display.setTextSize(2);
      display.print("C");

      // display humidity
      display.setTextSize(1);
      display.setCursor(0, 35);
      display.print("Humidity: ");
      display.setTextSize(2);
      display.setCursor(0, 45);
      display.print(humi);
      display.print(" %");

      display.display();
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }

  // wait a 2 seconds between readings
  delay(2000);
}

//  GET request function
String httpGETRequest(const char* serverName) {

   // define request header format
   https.addHeader("Content-Type", "application/x-www-form-urlencoded");

   //----------------------------------------Prepare HTTP Request
   httpRequestData="dht_sensor_API_KEY=" + String(API_KEY) + "&temperature=" + String(tempC) + "&humidity=" + String(humi) + "";
   // Send POST request
   httpResponseCode=https.POST(httpRequestData);
   Serial.print("HTTP Response code: ");
   Serial.println(httpRequestData);
   //----------------------------------------

  //----------------------------------------GET HTTP Request
  if (httpResponseCode == 200) { //initialize payload if GET data is available
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

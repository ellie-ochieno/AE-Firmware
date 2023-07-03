#include <DHT.h>
#include <WiFi.h>
//#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// sensor readings
float humi  = 0;
float tempC = 0;
// set threshods
int humid_threshold;
int temp_threshold;

#define DHT_SENSOR_TYPE DHT22
DHT dht_sensor(DHT_PIN, DHT_SENSOR_TYPE);

const char* server_url="https://smart-home-iot.angazaelimu.com/api/dht_data_insert";

WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;
String payload;
int httpResponseCode;
String httpRequestData;

void setup() {
  Serial.begin(115200);

  // setting up custom I2C pins
  Wire.begin(I2C_SDA, I2C_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

  display.clearDisplay();
  display.setTextColor(WHITE);

  pinMode(INA_PIN, OUTPUT);
  pinMode(INB_PIN, OUTPUT);

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
 if(client){
    // set secure client via root cert
    client->setCACert(root_cacert);
    //create an HTTPClient instance

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){

      // read humidity
      humi  = dht_sensor.readHumidity();
      // read temperature in Celsius
      tempC = dht_sensor.readTemperature();
    
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
      Serial.println("Temperature threshold: " + String(temp_threshold) + "\n");
    
      // check whether the reading is successful or not
      if ( isnan(tempC) || isnan(humi)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
    
      if (tempC>(temp_threshold)){
          digitalWrite(INA_PIN,LOW);
          Serial.println("spinning");
          digitalWrite(INB_PIN,HIGH);
          delay(5000);
      }else{
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

   // Prepare POST request data
   httpRequestData="dht_sensor_API_KEY=" + String(API_KEY) + "&temperature=" + String(tempC) + "&humidity=" + String(humi) + "";

   // Send POST request
   httpResponseCode=https.POST(httpRequestData);

   Serial.print("HTTP Response code: ");
   Serial.println(httpRequestData);

  //get the response
  if (httpResponseCode == 200) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload=https.getString();
    Serial.println(payload);
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

}

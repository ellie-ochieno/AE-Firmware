 /*
* SMART DOOR CONTROL-------------------
* Use PUSH BUTTONS:
*  -> Control door opening
*     - Use keypad to input password
*     - Compare the set password with platform set
* AMGAZA ELIMU - SH.PROJECT V2.0 / 023
* ----------------------------------------
*/
//----------------------------------------Support libraries and sensor parameters.
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Keypad.h>
#include "secrets.h"
#include "Tone32.hpp"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include "pin_configurations.h"
#include "ESP32_New_ISR_Servo.h"

#define TIMER_INTERRUPT_DEBUG  0
#define ISR_SERVO_DEBUG   1
// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#define USE_ESP32_TIMER_NO  3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define PWM_CHANNEL 0
// Published values for SG90 servos; adjust if needed
#define MIN_MICROS  544 //544
#define MAX_MICROS  2450
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // three columns

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Tone32 object for buzzer
Tone32 tone32(BUZZER_PIN, PWM_CHANNEL);
//----------------------------------------

//----------------------------------------Control variables.
const char* server_url="https://smart-home-iot.angazaelimu.com/api/door_state_control";
char key;
String door_state;
String door_data;
String door_control_status = "0";

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {18, 5, 17, 16}; // GPIO18, GPIO5, GPIO17, GPIO16 connect to the row pins
byte pin_column[COLUMN_NUM] = {4, 0, 2};  // GPIO4, GPIO0, GPIO2 connect to the column pins

  // kypad object
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

HTTPClient https;
int httpResponseCode;
String set_password; // To hold platform set password
String input_password;  // To hold keypad input password
String httpRequestData;
  // controls init
int wrong_tone_frequency = 600;
int right_tone_frequency = 1200;
WiFiClientSecure *client = new WiFiClientSecure;
//----------------------------------------


void setup() {
  Serial.begin(9600);
  input_password.reserve(32); // maximum input characters is 33, change if needed
  Serial.begin(115200);
  while (!Serial);
    delay(500);

  //Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);
  servoIndex1 = ESP32_ISR_Servos.setupServo(SERVO_PIN, MIN_MICROS, MAX_MICROS);
  if (servoIndex1 != -1)
    Serial.println(F("Setup Servo1 OK"));
  else
    Serial.println(F("Setup Servo1 failed"));

  // setting up custom I2C pins
  Wire.begin(I2C_SDA, I2C_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(200);

  //Display Smart Home
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.print("Smart Home");
  display.display();
  delay(200);

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
  //read from keypad
  key = keypad.getKey();


 if(client){
                                   // set secure client via root cert
    client->setCACert(root_cacert);
                                   //create an HTTPClient instance
                                   //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){

                                    // call http server handler function
      door_data = httpGETRequest(server_url);
      
                                    // get json response data
      JSONVar json_res_data = JSON.parse(door_data);

                                    // validate response type
      if (JSON.typeof(json_res_data) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
                                    // decode actual led status data
      door_state = (const char*)(json_res_data["push_btn_state"]);
      set_password = (const char*)(json_res_data["door_password"]);
                                    // print server received data
      Serial.println("\nDoor state: " + door_state);
      Serial.println("\nDoor password: " + set_password);
      
      if (key) {                    // if keypad is pressed
        Serial.println(key);
      
        if (key == '*') {           // initialize keypad to receiving new inputs mode
          input_password = "";      // clear input password
        } else if (key == '#') {    // if keypad 'okay' btn is pressed
          if(set_password == input_password){
                                    // change door control status if correct password
            door_control_status = "1";
          }
                                    // Calls door state control handler
          doorControl(door_state, set_password, input_password);
      
          input_password = "";      // clear input password
        } else {
          input_password += key;    // append new character to input password string
        }
      }
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  // wait a 2 seconds between readings
  // delay(20);
  
}


//  GET request function
String httpGETRequest(const char* serverName) {
  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  //----------------------------------------Prepare HTTP Request
  httpRequestData = "pbtn_API_KEY=" + String(API_KEY) + "&pbtn_status=" + door_control_status + "";
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);
  Serial.print("\nData request: ");
  Serial.print(httpRequestData);
  Serial.println("");
  //----------------------------------------

  //----------------------------------------GET HTTP Request
  if (httpResponseCode == 200) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();
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
//Control door based on platform set parameters
void doorControl(String door_ctrl_state, String door_password, String input_password){
  if(door_ctrl_state == "1" || door_password == input_password){//door open controls and signals
    tone32.playTone(right_tone_frequency);
    Serial.println("The password is correct");
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,20);
    display.print("Password:");
    display.setTextSize(2);
    display.setCursor(0,40);
    display.print("Right");
    display.display();
                              // invoke door opening
    if (servoIndex1 != -1 ){
      for (position = 0; position <= 180; position+=10){
        // goes from 0 degrees to 180 degrees
        // in steps of 10 degree
        Serial.print(F("Servo1 pos = ")); Serial.print(position);
        ESP32_ISR_Servos.setPosition(servoIndex1, position);

        // waits 30ms for the servo to reach the position
        delay(30);
      }

      delay(1000);

      for (position = 180; position >= 0; position-=10){

          Serial.print(F("Servo1 pos = ")); Serial.print(position);

          ESP32_ISR_Servos.setPosition(servoIndex1, position);

        // waits 30ms for the servo to reach the position
        delay(30);
      }
      delay(1000);
      tone32.stopPlaying();
    }
  }
  else{
      tone32.playTone(wrong_tone_frequency);
      Serial.print("The password is wrong");
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0,20);
      display.print("Password:");
      display.setTextSize(2);
      display.setCursor(0,40);
      display.print("Error");
      display.display();
      delay(1000);
      tone32.stopPlaying();
      display.clearDisplay();
      display.setCursor(0,20);
      display.print(" Try Again ");
      display.display();
    }
  }

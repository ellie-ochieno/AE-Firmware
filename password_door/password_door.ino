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

#define PWM_CHANNEL 0
// Published values for SG90 servos; adjust if needed
#define MIN_MICROS  544 //544
#define MAX_MICROS  2450
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // three columns
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define ISR_SERVO_DEBUG   1
// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#define USE_ESP32_TIMER_NO  3
#define TIMER_INTERRUPT_DEBUG  0

// Tone32 object for buzzer
Tone32 tone32(BUZZER_PIN, PWM_CHANNEL);
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//----------------------------------------

//----------------------------------------Control variables.
char key;
int position;
int countflag;
String payload;
HTTPClient https;
String door_data;
String door_state;
String door_status;  
int servoIndex1 = -1;
String set_password; // To hold platform set password
int httpResponseCode;
String input_password;  // To hold keypad input password
String httpRequestData;
int wrong_tone_frequency = 600;// buzzer control tone
int right_tone_frequency = 1200;// buzzer control tone
String door_control_status = "0";
const char* server_url="https://smart-home-iot.angazaelimu.com/api/door_state_control";

// keypad configuration dfn
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROW_NUM] = {23, 22, 4, 21}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte colPins[COLUMN_NUM] = {19, 18, 5, 17};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

// kypad object
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROW_NUM, COLUMN_NUM );
// wifi handler object
WiFiClientSecure *client = new WiFiClientSecure;
//----------------------------------------

void setup() {
  Serial.begin(115200);
  input_password.reserve(32); // maximum input characters is 33, change if needed

  //Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);
  servoIndex1 = ESP32_ISR_Servos.setupServo(SERVO_PIN, MIN_MICROS, MAX_MICROS);
  if (servoIndex1 != -1)
    Serial.println(F("Setup Servo1 OK"));
  else
    Serial.println(F("Setup Servo1 failed"));

  // setting up custom I2C pins
  // Wire.begin(I2C_SDA, I2C_SCL);

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
  Serial.print("\nconnecting...");
  delay(2000);
  Serial.print("\nServer connected!");
  delay(1000);
  Serial.print("\n\nInitialising password input field ");
  eventInitialize(2000);  // invoke input fields initializer
  Serial.print("\n\nEnter door password ");
  Serial.print("\n->Press # to submit or");
  Serial.print("\n->Press * to clear password and start new entries");
  Serial.print("\nPassword: ");
}

void loop() {
  //read from keypad
  key = keypad.getKey();

                                  // initialising keypad for pasword entries  
  if (key) {                      // if keypad is pressed
    Serial.print(key);
  
    if (key == '*') {             // initialize keypad to receiving new inputs mode
      input_password = "";        // clear input password
    } 
    else if (key == '#') {        // if keypad 'okay' btn is pressed
      Serial.print("\nOk!");
      Serial.print("\n\nEntered password: "+input_password+"\n");
      
                                  // Initialize password verification
      Serial.print("\nVerifying password ");
      eventInitialize(2000);  // invoke input fields initializer
      Serial.print("\nDone!\n ");
      Serial.print("\nLoading server information...(please wait!)\n ");
      
      secureClientSetup();        // invokes client setup handler
      
      if(set_password == input_password){// if password correct
                                  // authentication confirm
        Serial.print("\nPASSWORD AUTHENTICATION STATUS-----------\n");
        Serial.print("-Success! \n-Password correct.\n");
        Serial.print("\n-----------------------------------------\n");
        
                                  // Call door state control handler
        doorControl(door_state, set_password, input_password);

        if(httpResponseCode == 200){ // if success connection 
          Serial.print("\n\n---------------SUCCESS!!------------------\n");
        }

        // Next sequence
        Serial.print("\n------------------------------------------\n");
        if(httpResponseCode != 200){// if connection error
          Serial.print("\nRe-loading input field");
        }
        if(httpResponseCode == 200){
          Serial.print("\nLoading next sequence");
        }
        eventInitialize(2000);  // initializing next seqoence
        
        // prompt for password to close the door
        if(door_status == "Open!"){
          Serial.println("\n\nDoor state: " + door_status);
          Serial.print("\nEnter password to close the door.");
          Serial.print("\n->Press # to submit or");
          Serial.print("\n->Press * to clear password and start new entries");
          Serial.print("\nPassword: ");
        }
        // prompt for password to open the door
        if(door_status == "Closed!"){
          Serial.println("\n\nDoor state: " + door_status);
          Serial.print("\nEnter password to open the door.");
          Serial.print("\n->Press # to submit or");
          Serial.print("\n->Press * to clear password and start new entries");
          Serial.print("\nPassword: ");
        }
      } 
      else {// if password wrong
        Serial.print("\nPASSWORD AUTHENTICATION STATUS-----------\n");
        Serial.print("\nIncorrect password! \n");
        Serial.print("\n-----------------------------------------\n");
        Serial.print("\nPlease try again.");
        Serial.print("\n->Press # to submit or");
        Serial.print("\n->Press * to clear password and start new entries");
        Serial.print("\nRe-enter password: ");
      }
  
      input_password = "";      // empty input password holder 
    } 
    else {
      input_password += key;    // append new character to input password string
    }
  }
}

/*
 * GET request function -----------------------------------------
 */
String httpGETRequest(const char* serverName) {
  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  if((set_password == input_password) && (door_status == "Closed!")){// set door status to HIGH if password correct
    door_control_status = "1";
  }
  if((set_password == input_password) && (door_status == "Open!")){// set door status to HIGH if password correct
    door_control_status = "0";
  }

  //----------------------------------------Prepare HTTP Request
  httpRequestData = "pbtn_API_KEY=" + String(API_KEY) + "&pbtn_status=" + door_control_status + "";
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);
  Serial.print("\nPOST request data: \n\t-API Key: "+String(API_KEY)+"\n\t-Door control signal: "+ door_control_status + "\n");
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

/*
 * Secure client setup handler -----------------------------------------
 */
void secureClientSetup(){

  if(client){
                                   // set secure client via root cert
    client->setCACert(root_cacert);
                                   //create an HTTPClient instance
                                   //Initializing an HTTPS communication using the secure client
    if(https.begin(*client, server_url)){
      if(set_password == input_password){// set door status to HIGH if password correct
        door_control_status = "1";
      }
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
                           
      if(door_state == "1"){
        door_status = "Open!";
      }
      else{
        door_status = "Closed!";
      }
      Serial.println("\nDoor state: " + door_status);
      Serial.println("Platform set door password: " + set_password);
      
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }  
}
  
/*
 * Smart door control handler -----------------------------------------
 */
void doorControl(String door_ctrl_state, String door_password, String input_password){
  if(door_ctrl_state == "1" || door_password == input_password){//if door password is correct and control signal HIGH
    tone32.playTone(right_tone_frequency);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,20);
    display.print("Password:");
    display.setTextSize(2);
    display.setCursor(0,40);
    display.print("Right");
    display.display();
    if (servoIndex1 != -1 ){
                                // invoke door opening if received open signal
      if(door_control_status == "1"){   
        if(httpResponseCode != 200){// in case of server connection error!
          Serial.print("\nServer connection error!\nPlease try again. ");
        } 
        else{
          Serial.print("\nInitialising door opening ");
          eventInitialize(1000);  // invoke event initializer
          Serial.print("\n");
          for (position = 0; position <= 180; position+=10){
            // goes from 0 degrees to 180 degrees
            // in steps of 10 degree
            Serial.print(F("Servo1 pos = ")); Serial.print(position);
            ESP32_ISR_Servos.setPosition(servoIndex1, position);
    
            // waits 30ms for the servo to reach the position
            delay(30);
          }
        }
        delay(1000);
      }
                                // invoke door closing if received close signal
      if(door_control_status == "0"){ 
        if(httpResponseCode != 200){// in case of server connection error!
          Serial.print("\nServer connection error!\nPlease try again. ");
        } 
        else{
          Serial.print("\nInitialising door closing ");
          eventInitialize(1000);  // invoke event initializer
          Serial.print("\n");
          for (position = 180; position >= 0; position-=10){
    
              Serial.print(F("Servo1 pos = ")); Serial.print(position);
    
              ESP32_ISR_Servos.setPosition(servoIndex1, position);
    
            // waits 30ms for the servo to reach the position
            delay(30);
          }
        }
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

 // control event initialising handler
 void eventInitialize(int ctl_time){
    delay(ctl_time);
    countflag = 0;       // reset count control flag
    while(countflag<=2){ // initialising input fields
      Serial.print(".");
      countflag++;
      delay(ctl_time);
    }
  }

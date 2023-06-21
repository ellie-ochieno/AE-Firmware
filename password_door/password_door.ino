#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include "Tone32.hpp"
#include "ESP32_New_ISR_Servo.h"
#include "secrets.h"
#include "pin_configurations.h"

#define TIMER_INTERRUPT_DEBUG       0
#define ISR_SERVO_DEBUG             1

// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#define USE_ESP32_TIMER_NO          3


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define PWM_CHANNEL 0

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Tone32 object for buzzer
Tone32 tone32(BUZZER_PIN, PWM_CHANNEL);

const char* URL="https://smart-home-iot.angazaelimu.com/api/door_state_control";
int red_button_state;
int green_button_state;
int Red_num_time;
String password;
int door_flag;
String payload;
int key_door_flag;
int password_length_flag;
double right_tone_frequency;
double wrong_tone_frequency;

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS      544 //544
#define MAX_MICROS      2450

int servoIndex1  = -1;

WiFiClientSecure client;
HTTPClient https;

/*
 * After completing the password input, if the green button is pressed,
 * the password will be confirmed. If the final password value is equal to ".--.",
 * the password is correct, otherwise the password is wrong.
 */
void Password_Confirmation()
{
  delay(100);
  if ((green_button_state == 0) && (red_button_state != 0))
  {
      if (key_door_flag == 0) {
        Serial.println("Checking the key door flag");
        if (password == ".--.")
        {
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
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          int httpResponseCode=https.POST("pbtn_API_KEY=" + String(API_KEY) + "&pbtn_status=1");
          int position;

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
          delay(200);
          key_door_flag = 1;
          delay(1000);
          tone32.stopPlaying();
        }
          }
        else {
          tone32.playTone(wrong_tone_frequency);
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          int httpResponseCode=https.POST("pbtn_API_KEY=" + String(API_KEY) + "&pbtn_status=0");
          Serial.print("The password is wrong");
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          //display.setCursor(0,5);
          //display.print("Smart Home ");
          display.setCursor(0,20);
          display.print("Password:");
          display.setTextSize(2);
          display.setCursor(0,40);
          display.print("Error");
          display.display();
          delay(1000);
          tone32.stopPlaying();
          //tone(3,165);
          //delay(125);
          //delay(500);
          //noTone(3);
          //delay(200);
          display.clearDisplay();
          display.setCursor(0,20);
          display.print(" Try Again ");
          display.display();
          //key_voice();
          key_door_flag = 1;
        }

      }
      password = "";


      if (key_door_flag == 1) {
        delay(1000);
        key_door_flag = 0;
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0,28);
        display.print("Smart Home ");
        display.display();

      }

  }
}


void setup(){
  Serial.begin(115200);
  while (!Serial);
    delay(500);

  Red_num_time = 0;
  password = "";
  key_door_flag = 0;
  password_length_flag = 0;
  wrong_tone_frequency = 600;
  right_tone_frequency = 1200;


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

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi ...");
  }

  Serial.println("Connected to the WiFi network");

  pinMode(BUTTON_RED, INPUT);
  pinMode(BUTTON_GREEN, INPUT);
}

void loop()
{
    https.begin(client, URL);
    // The green button is connected to the digital port D4,
    //the program block outputs the state of the button,
    //the high level "1" represents the button is not pressed,
    //and the low level "0" represents the button is pressed
    green_button_state = digitalRead(BUTTON_GREEN);

    // The red button is connected to the digital port D5,
    //the program block outputs the state of the button,
    //the high level "1" represents the button is not pressed,
    //and the low level "0" represents the button is pressed
    red_button_state = digitalRead(BUTTON_RED);


    if ((green_button_state != 0) && (red_button_state == 0) )
    {
      Serial.println("The green button state is:");
      Serial.println(green_button_state);
      Serial.println("The red button state is:");
      Serial.println(red_button_state);
      delay(100);
      green_button_state  = digitalRead(BUTTON_GREEN);
      while ((green_button_state != 0) && (red_button_state == 0))
      {
        red_button_state = digitalRead(BUTTON_RED);
        Red_num_time = Red_num_time + 1;
        delay(100);
      }

    }
    if ((Red_num_time > 1) && (Red_num_time < 5))
    {
      //key_voice();
      password = String(password) + String(".");
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0,5);
      display.print("Password :");
      display.setTextSize(2);
      display.setCursor(28,20);
      display.print(password);
      display.display();

    }
    if (Red_num_time > 5)
    {
      //key_voice();
      password = String(password) + String("-");
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0,5);
      display.print("Password :");
      display.setTextSize(2);
      display.setCursor(20,20);
      display.print(password);
      display.display();

    }

    if ((password.length() == 4)||(password.length() > 4))
    {
      password_length_flag = 1;
    }

    switch (password_length_flag)
    {
      case 0:
       password_length_flag = 0;
      break;
      case 1:
        Password_Confirmation();
        password_length_flag = 0;
        Serial.println("Finished Password Confirmation");
      break;
    }


    Red_num_time = 0;
    Serial.println("Gotten to the end of the loop");
    Serial.println(password);

    //delay(5000);
}


//  GET request function
String httpGETRequest(const char* serverName) {

  // Initialize http protocol

  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Prepare POST request data
  String httpRequestData = "pbtn_API_KEY=" + String(API_KEY);

  // Send POST request
  int httpResponseCode = https.POST(httpRequestData);


  //get the response
  if (httpResponseCode == 200) { //initialize payload if GET data is available
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    //payload=https.getString();
    //Serial.println(payload);

  }
  else {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }


}
#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Tone32.hpp"
#include <ESP32_Servo.h>
const int servoPin = 4; // Pin connected to servo
Servo doorServo;
int pos = 0;

#define TIMER_INTERRUPT_DEBUG       0
#define I2C_SCL 2  //SCL
#define I2C_SDA 42  //SDA
#define Green 19
#define Red 33


// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#define USE_ESP32_TIMER_NO          3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define PWM_CHANNEL 0
#define BUZZER_PIN 5
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Tone32 object for buzzer
Tone32 tone32(BUZZER_PIN, PWM_CHANNEL);
double right_tone_frequency;
double wrong_tone_frequency;

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // three columns
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pin_rows[ROW_NUM] = {1, 10, 9, 11}; // GPIO18, GPIO5, GPIO17, GPIO16 connect to the row pins
byte pin_column[COLUMN_NUM] = {12, 14, 21, 34};  // GPIO4, GPIO0, GPIO2 connect to the column pins
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
const String password = "6908"; // change your password here
String input_password;
// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#define USE_ESP32_TIMER_NO          3
void setup() {
  Serial.begin(9600);
  pinMode(Green,  OUTPUT);
  pinMode(Red, OUTPUT);
  doorServo.attach(servoPin);
  input_password.reserve(32); // maximum input characters is 33, change if needed
  wrong_tone_frequency = 600;
  right_tone_frequency = 1200;
  // setting up custom I2C pins
  Wire.begin(I2C_SDA, I2C_SCL);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(200);
  //Display Smart Home
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 28);
  display.print("Smart Home");
  display.display();
  delay(200);
}
void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    if (key == '*') {
      input_password = ""; // clear input password
    } else if (key == '#') {
      if (password == input_password) {
        Serial.println("The password is correct, ACCESS GRANTED!");
        // DO YOUR WORK HERE
        tone32.playTone(right_tone_frequency);
        openDoor();

        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(0, 20);
        display.print("Password:");
        display.setTextSize(2);
        display.setCursor(0, 40);
        display.print("Right");
        display.display();
        delay(1000);
        

        delay(5000); // Keep the door open for 5 seconds
        closeDoor();
        tone32.stopPlaying();
         display.clearDisplay();
        display.setCursor(0, 20);
        display.setTextSize(1);
        display.print(" Input Password"); // Print "Input Password" after "Try Again"
        display.display();
      } else {
        Serial.println("The password is incorrect, ACCESS DENIED!");
        tone32.playTone(wrong_tone_frequency);
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(0, 20);
        display.print("Password:");
        display.setTextSize(2);
        display.setCursor(0, 40);
        display.print("Error");
        display.display();
        delay(1000);
        tone32.stopPlaying();
        display.clearDisplay();
        display.setCursor(0, 20);
        display.print(" Try Again ");
        delay(2000);
        display.display();
        display.clearDisplay();
        display.setCursor(0, 20);
        display.setTextSize(1);
        display.print(" Input Password"); // Print "Input Password" after "Try Again"
        display.display();

      }
      input_password = ""; // clear input password
    } else {
      input_password += key; // append new character to input password string
    }
  }
}
void openDoor() {
  Serial.print("Opening Door");
  digitalWrite(Green,HIGH);
  delay(2000);
  digitalWrite(Green,LOW);
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    doorServo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);   
              // waits 15ms for the servo to reach the position
  }

}

void closeDoor() {
   digitalWrite(Red,HIGH);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    doorServo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
}

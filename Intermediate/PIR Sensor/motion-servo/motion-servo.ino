#include <Servo.h>

const int PIR_PIN = 15;     // GPIO 15 pin connected to OUTPUT pin of sensor
const int LED_PIN = 2;      // GPIO 2 pin connected to OUTPUT pin of LED
const int SERVO_PIN = 18;   // GPIO 18 pin connected to the control pin of the servo motor

Servo myServo;              // Create a servo object to control the servo motor

int pinStateCurrent = LOW;   // current state of pin
int pinStatePrevious = LOW;  // previous state of pin

void setup() {
  Serial.begin(115200);              // initialize serial
  pinMode(PIR_PIN, INPUT);         // set ESP32 pin to input mode to read value from OUTPUT pin of sensor
  pinMode(LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);       // attach the servo object to the specified pin
}

void loop() {
  pinStatePrevious = pinStateCurrent;    // store old state
  pinStateCurrent = digitalRead(PIR_PIN); // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {
    Serial.println("Motion detected!");
    digitalWrite(LED_PIN, HIGH);
    moveServo(180);  // Rotate the servo 180 degrees
    // TODO: Add additional actions when motion is detected
  } else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {
    Serial.println("Motion stopped!");
    digitalWrite(LED_PIN, LOW);
    moveServo(0);  // Rotate the servo back to 0 degrees
    // TODO: Add additional actions when motion stops
  }
}

void moveServo(int angle) {
  myServo.write(angle);  // Set the angle of the servo motor
  delay(1000);           // Delay to allow the servo to reach the desired position
}

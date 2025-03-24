#include <Arduino.h>

const int buttonPin = 2;
const int ledPin = 4;

int buttonState = HIGH;
int ledState = LOW;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != buttonState) {
    buttonState = reading;

    if (buttonState == LOW) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      Serial.println("LED state toggled!");
    }
  }
}

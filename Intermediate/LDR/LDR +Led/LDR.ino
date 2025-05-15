#Include "pin_configurations.h"
#define LDR_PIN
#define RED_PIN

const int LIGHT_SENSOR_PIN = LDR_PIN;
const int LED_PIN = RED_PIN;
const int ANALOG_THRESHOLD = 300;
int analogValue;
void setup() {
pinMode(LED_PIN, OUTPUT);
}
void loop() {
analogValue = analogRead(LIGHT_SENSOR_PIN);
if(analogValue < ANALOG_THRESHOLD){
digitalWrite (LED_PIN, HIGH);
}
Else{
digitalWrite(LED_PIN, LOW);}
}

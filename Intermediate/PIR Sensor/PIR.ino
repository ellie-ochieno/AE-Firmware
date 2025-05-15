/*
This is the sample code for the PIR module
It detects if there is motion detected or not
Can utilise LED
 */

const int PIR_PIN = 39; // GIOP18 pin connected to OUTPUT pin of sensor
const int LED_PIN = 9; // GIOP13 pin connected to OUTPUT pin of LED
int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin

void setup() {
  Serial.begin(9600);            // initialize serial
  pinMode(PIR_PIN, INPUT); // set ESP32 pin to input mode to read value from OUTPUT pin of sensor
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  pinStatePrevious = pinStateCurrent; // store old state
  pinStateCurrent = digitalRead(PIR_PIN);   // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    digitalWrite(LED_PIN, HIGH);
    // TODO: turn on alarm, light or activate a device ... here
  }
  else
  if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    digitalWrite(LED_PIN, LOW);
    // TODO: turn off alarm, light or deactivate a device ... here
  }
}

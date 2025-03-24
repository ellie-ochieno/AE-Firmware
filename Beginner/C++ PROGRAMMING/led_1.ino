const int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Read the current state of the LED pin
  int ledState = digitalRead(ledPin);

  // Check the current state of the LED
  if (ledState == LOW) {
    // If the LED is currently off, turn it on
    digitalWrite(ledPin, HIGH);
    delay(1000); // Wait for 1 second
  } else {
    // If the LED is currently on, turn it off
    digitalWrite(ledPin, LOW);
    delay(1000); // Wait for 1 second
  }
}

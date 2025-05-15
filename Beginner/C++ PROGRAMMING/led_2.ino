// Define the onboard LED pin
const int LED_PIN = 13;

void setup() {
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Blink the LED on and off for 5 cycles
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);  // Turn the LED on
    delay(500);                   // Wait for 500 milliseconds
    digitalWrite(LED_PIN, LOW);   // Turn the LED off
    delay(500);                   // Wait for 500 milliseconds
  }
  delay(3000); //wait for three seconds
}

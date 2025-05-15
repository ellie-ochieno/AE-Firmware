// Define the constants
const int BUTTON_PIN = 4; // GPIO19
const int LED_PIN = 9;    // On-board LED

// Define the variables
int button_state = 0; // Variable for reading the button status

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  // Initialize the button pin as an input with pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Read the state of the button value
  button_state = digitalRead(BUTTON_PIN);

  // Control LED according to the state of the button
  if (button_state == HIGH)     // If button is pressed (input pulled to ground)
    digitalWrite(LED_PIN, LOW); // Turn off LED
  else                           // Otherwise, button is not pressed
    digitalWrite(LED_PIN, HIGH); // Turn on LED
}
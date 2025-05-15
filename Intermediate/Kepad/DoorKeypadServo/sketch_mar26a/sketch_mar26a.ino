#include <Keypad.h>
#include <ESP32_Servo.h> 

const int servoPin = 13; // Pin connected to servo
Servo doorServo;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {1, 10, 9, 11}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 14, 21, 34}; // Connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String password = "1234"; // Define the password

void setup() {
  Serial.begin(9600);
  doorServo.attach(servoPin);
  pinMode(servoPin, OUTPUT);
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    static String enteredPassword = ""; // Store entered keys
    if (key == '#') { // Check if enter key is pressed
      if (enteredPassword == password) { // Check if entered password is correct
        Serial.println("Correct Password. Opening Door.");
        openDoor();
        delay(5000); // Keep the door open for 5 seconds
        closeDoor();
      } else {
        Serial.println("Incorrect Password. Try Again.");
      }
      enteredPassword = ""; // Clear entered password
    } else {
      enteredPassword += key; // Append entered key
      Serial.print("Entered Password: ");
      Serial.println(enteredPassword);
    }
  }
}

void openDoor() {
  doorServo.write(90); // Open the door
}

void closeDoor() {
  doorServo.write(0); // Close the door
}

#include <Keypad.h>
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // three columns
char keys[ROW_NUM][COLUMN_NUM] = {
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};
byte pin_rows[ROW_NUM] = {18, 5, 17, 16}; // GPIO18, GPIO5, GPIO17, GPIO16 connect to the row pins
byte pin_column[COLUMN_NUM] = {4, 0, 2};  // GPIO4, GPIO0, GPIO2 connect to the column pins
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
const String password = "7890"; // change your password here
String input_password;
void setup() {
 Serial.begin(9600);
 input_password.reserve(32); // maximum input characters is 33, change if needed
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
     } else {
       Serial.println("The password is incorrect, ACCESS DENIED!");
     }
     input_password = ""; // clear input password
   } else {
     input_password += key; // append new character to input password string
   }
 }
}

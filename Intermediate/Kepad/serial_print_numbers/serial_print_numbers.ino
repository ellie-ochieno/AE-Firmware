#include <Keypad.h>
#define ROW_NUM   4 // four rows
#define COLUMN_NUM  4 // four columns
char keys[ROW_NUM][COLUMN_NUM] = {
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'}
};
byte pin_rows[ROW_NUM]    = {1, 10, 9, 11}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {12, 14, 21, 34};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
void setup() {
 Serial.begin(9600);
}


void loop() {
 char key = keypad.getKey();


 if (key) {
 Serial.println(key);
 }
}

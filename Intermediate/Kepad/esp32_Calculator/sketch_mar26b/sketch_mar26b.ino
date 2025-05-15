#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'7','8','9','D'},
  {'4','5','6','C'},
  {'1','2','3','B'},
  {'*','0','#','A'}
};

byte rowPins[ROWS] = {1, 10, 9, 11}; // Connect to ESP32 GPIO pins
byte colPins[COLS] = {12, 14, 21, 34}; // Connect to ESP32 GPIO pins

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

long Num1, Num2, Number;
char key, action;
boolean result = false;

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Calculator");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  key = kpd.getKey();
  if (key != NO_KEY)
    DetectButtons();
  if (result == true)
    CalculateResult();
  DisplayResult();
}

void DetectButtons() {
  if (key == '*') {
    Serial.println("Button Cancel");
    Number = Num1 = Num2 = 0;
    result = false;
  }
  else if (key >= '0' && key <= '9') {
    Serial.print("Button ");
    Serial.println(key);
    Number = (Number * 10) + (key - '0');
  }
  else if (key == '#') {
    Serial.println("Button Equal");
    Num2 = Number;
    result = true;
  }
  else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    Num1 = Number;
    Number = 0;
    if (key == 'A') {
      Serial.println("Addition");
      action = '+';
    }
    else if (key == 'B') {
      Serial.println("Subtraction");
      action = '-';
    }
    else if (key == 'C') {
      Serial.println("Multiplication");
      action = '*';
    }
    else if (key == 'D') {
      Serial.println("Division");
      action = '/';
    }
    delay(100);
  }
}

void CalculateResult() {
  if (action == '+')
    Number = Num1 + Num2;
  else if (action == '-')
    Number = Num1 - Num2;
  else if (action == '*')
    Number = Num1 * Num2;
  else if (action == '/') {
    if (Num2 != 0)
      Number = Num1 / Num2;
    else
      Number = 0; // Error handling for division by zero
  }
}

void DisplayResult() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(Num1);
  display.print(action);
  display.print(Num2);

  if (result == true) {
    display.print(" =");
    display.print(Number);
  }

  display.setCursor(0, 16);
  display.print(Number);
  display.display();
}

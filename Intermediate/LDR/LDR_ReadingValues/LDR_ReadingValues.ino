#Include "pin_configurations.h"
#define LDR_PIN
#define RED_PIN



void setup() {
  // initialize serial communication
  Serial.begin(9600);
}

void loop() {
  int lightValue = analogRead(LDR_PIN);

  Serial.print("The AO value: ");
  Serial.println(lightValue);
}

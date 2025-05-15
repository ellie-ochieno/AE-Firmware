#define POWER 33
#define SIGNAL 32
#define BUZZER 18

int value = 0;
int level = 0;

void setup() {
  Serial.begin(115200);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, LOW);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  level = waterSensor();
  Serial.print("Water Level: ");
  Serial.println(level);
  delay(5000);

  if (level > 450) {
    activateBuzzer();
  } else {
    deactivateBuzzer();
  }
}

void activateBuzzer() {
  tone(BUZZER, 1000);  // Adjust the frequency as needed
  delay(5000);          // Adjust the duration as needed
  noTone(BUZZER);       // Turn off the buzzer after a short delay
}


void deactivateBuzzer() {
  digitalWrite(BUZZER, LOW);
}

int waterSensor() {
  digitalWrite(POWER, HIGH);
  delay(10);
  value = analogRead(SIGNAL);
  delay(10);
  digitalWrite(POWER, LOW);
  return value;
}

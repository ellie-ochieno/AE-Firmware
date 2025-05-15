const int GAS_PIN = 20;    
const int LED_PIN = 9;
const int TONE_PIN = 15;


// The ESP32 has 16 channels which can generate 16 independent waveforms
// We'll just choose PWM channel 0 here
const int TONE_PWM_CHANNEL = 0;


void setup() {
  Serial.begin(115200);
  ledcAttachPin(TONE_PIN, TONE_PWM_CHANNEL);  
  pinMode(GAS_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}


void loop() {
 
  int gasLevel = analogRead(GAS_PIN);

  Serial.print("Gas Level: ");
  Serial.print(gasLevel);
  Serial.print("\t");
  Serial.print("\t");
 
  if (gasLevel> 1000) {
    Serial.println("Gas");
    digitalWrite (LED_PIN, HIGH) ; //turn LED On
    ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
    delay(1000);
    digitalWrite (LED_PIN, LOW) ;  //turn Off LED
  }
  else {
    Serial.println("No Gas");
  }
  delay(100);
}

#define POWER 33
#define SIGNAL 32

int value = 0;
int level = 0;


void setup() {
 
 Serial.begin(115200);
 pinMode(POWER, OUTPUT);
digitalWrite(POWER, LOW);


}

void loop() {
  level=waterSensor();
  Serial.print("Water Level: ");
  Serial.println(level);
  delay(5000);

}



int waterSensor()
{
  digitalWrite(POWER, HIGH);
  delay(10);
  value=analogRead(SIGNAL);
  delay(10);
  digitalWrite(POWER, LOW);
  return value;
}
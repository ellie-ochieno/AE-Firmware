#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pin_configurations.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



#define DHT_SENSOR_TYPE DHT22
DHT dht_sensor(DHT_PIN, DHT_SENSOR_TYPE);

void setup() {
  Serial.begin(115200);

  // setting up custom I2C pins
  Wire.begin(I2C_SDA, I2C_SCL); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  
  display.clearDisplay();
  display.setTextColor(WHITE);

  
  pinMode(INA_PIN, OUTPUT);
  pinMode(INB_PIN, OUTPUT);

  dht_sensor.begin(); // initialize the DHT sensor
  

}

void loop() {  
  
  // read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();

 
  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
}

  if (tempC>25){
      digitalWrite(INA_PIN,LOW);
      Serial.println("spinning");
      digitalWrite(INB_PIN,HIGH);
      delay(5000);
  }else{
      digitalWrite(INA_PIN,LOW);
      Serial.println("stopped");
      digitalWrite(INB_PIN,LOW);
  }


  display.clearDisplay();  //clear display
  //print temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,16);
  display.print(tempC);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(humi);
  display.print(" %"); 
  
  display.display(); 
  
  
  // wait a 2 seconds between readings
  delay(10000);
}

#include <ESP32Servo.h>
#include "pin_configurations.h"
#define R_channel 0  
#define G_channel 1  
#define B_channel 2  
#define pwm_Frequency 5000 // pwm frequency  
#define pwm_resolution 8 // 8 bit resolution

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //Setup the LED
  ledcAttachPin(RED_PIN, R_channel);  
  ledcAttachPin(GREEN_PIN, G_channel);  
  ledcAttachPin(BLUE_PIN, B_channel);   
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution); 


}

void loop() {
     
  RGB_Color(100,200,100);
  delay(1000);
  RGB_Color(0,0,0);
  delay(1000);
  //http.end();
  RGB_Color(100,200,100);
  delay(1000);
  RGB_Color(0,0,0);
  delay(1000);
  //http.end();

}

void RGB_Color(int red, int green, int blue) {  
  ledcWrite(R_channel, red);   
  ledcWrite(G_channel, green);  
  ledcWrite(B_channel, blue);   
}  

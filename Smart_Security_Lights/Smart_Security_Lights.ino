#include <driver/i2s.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include "soc/i2s_reg.h"
#include "secrets.h"
#include "pin_configurations.h"

#define BUFLEN 256
int32_t audio_buf[BUFLEN];

int lightVal;   // light reading
String payload;
String httpRequestData;
int httpResponseCode;
float loudness;
// Setting PWM frequency, channels and bit resolution
const int FREQUENCY = 5000;
const int REDCHANNEL = 0;
const int GREENCHANNEL = 1;
const int BLUECHANNEL = 2;
const int RESOLUTION = 8;

static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number

static const i2s_config_t i2s_config = {
     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
     .sample_rate = 22050,
     .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
     .intr_alloc_flags = 0, // default interrupt priority
     .dma_buf_count = 8,
     .dma_buf_len = 64,
     .use_apll = false
};

static const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_WS,                   // BCKL
    .ws_io_num = I2S_SCK,                    // LRCL
    .data_out_num = I2S_PIN_NO_CHANGE,  // not used (only for speakers)
    .data_in_num = I2S_SD                  // DOUT
};

const char* server_url="https://smart-home-iot.angazaelimu.com/api/ldr_state_update";

WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;

void setup() {

  Serial.begin(115200);

  Serial.println("Configuring I2S...");

  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);   //install and start i2s driver
  REG_SET_BIT(I2S_TIMING_REG(i2s_num),BIT(9));   /*  #include "soc/i2s_reg.h"   I2S_NUM -> 0 or 1*/
  REG_SET_BIT(I2S_CONF_REG(i2s_num), I2S_RX_MSB_SHIFT);
  i2s_set_pin(i2s_num, &pin_config);
  delay(500);

  //setting up the WiFi
  WiFi.begin(SSID,PASSWORD);
  
  pinMode(ONBOARD_LED_PIN,OUTPUT);     //--> On Board LED port Direction output
  digitalWrite(ONBOARD_LED_PIN, HIGH); //--> Turn off Led On Board

  // initialize wifi connection
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ONBOARD_LED_PIN, LOW);
    delay(250);
    digitalWrite(ONBOARD_LED_PIN, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ONBOARD_LED_PIN, LOW); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  delay(1000);

  // server initialization
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  Serial.println("");
  Serial.print("connecting...");
  delay(1000);

  // Setup the LED channels
  ledcSetup(REDCHANNEL, FREQUENCY, RESOLUTION);
  ledcSetup(GREENCHANNEL,FREQUENCY, RESOLUTION);
  ledcSetup(BLUECHANNEL,FREQUENCY, RESOLUTION);

  ledcAttachPin(RED_PIN, REDCHANNEL);
  ledcAttachPin(GREEN_PIN, GREENCHANNEL);
  ledcAttachPin(BLUE_PIN, BLUECHANNEL);

  //Setting up the LDR
  pinMode(LDR_PIN, INPUT); // set ESP32 pin to input mode to read value from OUTPUT pin of sensor

}

void loop() {
 if(client){
    // set secure client via root cert
    client->setCACert(root_cacert);
    //create an HTTPClient instance

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if(https.begin(*client, server_url)){

      size_t bytes_read=0;
      i2s_read(i2s_num, &audio_buf, BUFLEN, &bytes_read, portMAX_DELAY);
      int32_t cleanBuf[BUFLEN / 2] {0};
      int cleanBufIdx = 0;
      for (int i = 0; i < BUFLEN; i++)
      {
        if (audio_buf[i] != 0)    // Exclude values from other channel
        {
            cleanBuf[cleanBufIdx] = audio_buf[i] >> 14;
            cleanBufIdx++;
        }
      }
      float meanval = 0;
      int volCount = 0;
      for (int i=0; i < BUFLEN / 2; i++)
      {
         if (cleanBuf[i] != 0)
         {
          meanval += cleanBuf[i];
          volCount++;
         }
      }
      meanval /= volCount;
    
      // subtract it from all sapmles to get a 'normalized' output
      for (int i=0; i< volCount; i++)
      {
          cleanBuf[i] -= meanval;
      }
    
      // find the 'peak to peak' max
      float maxsample, minsample;
      minsample = 100000;
      maxsample = -100000;
      for (int i=0; i<volCount; i++) {
        minsample = _min(minsample, cleanBuf[i]);
        maxsample = _max(maxsample, cleanBuf[i]);
      }
    
      loudness=maxsample - minsample;
    
      //Get the threshold value
      String ldr_data = httpGETRequest(server_url);
      JSONVar json_threshold=JSON.parse(ldr_data);
    
      // validate server Response
      if (JSON.typeof(json_threshold) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      int ldr_threshold=(const int)(json_threshold["ldr_threshold"]);
    
      Serial.println("LDR set treshold value: " + String(ldr_threshold));
    
      //Check the LDR reading
      lightVal = analogRead(LDR_PIN);
    
      if (lightVal>ldr_threshold){
    
          Serial.println(String(loudness));
    
          // if mean is greater than 1000 and LDR value is greater than the threshold,
          //turn on the LED
    
          if (loudness>2000){
             setColor(100,150,130);
             https.addHeader("Content-Type", "application/x-www-form-urlencoded");
             // Prepare POST request data
             httpRequestData = "ldr_sensor_API_KEY=" + String(API_KEY) + "&led_status=1" + "&ldr_status=1";
             httpResponseCode=https.POST(httpRequestData);
             https.end();
             return;
          }
          else{
             setColor(0,0,0);
             https.addHeader("Content-Type", "application/x-www-form-urlencoded");
             int httpResponseCode=https.POST("ldr_sensor_API_KEY=" + String(API_KEY) + "&led_status=0"+ "&ldr_status=1");
             //Serial.print("HTTP Response code: ");
             //Serial.println(httpResponseCode);
             https.end();
             return;
          }
       }
    
      else{
          setColor(0,0,0);
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          int httpResponseCode=https.POST("ldr_sensor_API_KEY=" + String(API_KEY) + "&led_status=0" + "&ldr_status=0");
          //Serial.print("HTTP Response code: ");
          //Serial.println(httpResponseCode);
          https.end();
          return;
    
      }
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  } 
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  delay(1000);
}

void setColor(int R, int G, int B) {
  ledcWrite(REDCHANNEL,   R);
  ledcWrite(GREENCHANNEL, G);
  ledcWrite(BLUECHANNEL,  B);
}


//  GET request function
String httpGETRequest(const char* serverName) {

  // Initialize http protocol
  //Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Prepare POST request data
  httpRequestData = "ldr_sensor_API_KEY=" + String(API_KEY) + "";
  // Send POST request
  httpResponseCode = https.POST(httpRequestData);

  //get the response
  if (httpResponseCode == 200) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload=https.getString();
    Serial.println(payload);

  }
  else {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  return payload;

}

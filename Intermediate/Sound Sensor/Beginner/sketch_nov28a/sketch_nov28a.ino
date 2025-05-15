#include <driver/i2s.h>
#include <Arduino.h>
#include "soc/i2s_reg.h"
#include "pin_configurations.h"

#define BUFLEN 256
int32_t audio_buf[BUFLEN];

float loudness;

const int LOUDNESS_THRESHOLD = 500;

// LED configuration
const int REDCHANNEL = 0;
const int GREENCHANNEL = 1;
const int BLUECHANNEL = 2;
const int FREQUENCY = 5000;
const int RESOLUTION = 8;

static const i2s_port_t i2s_num = I2S_NUM_0;
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
  .bck_io_num = I2S_WS,
  .ws_io_num = I2S_SCK,
  .data_out_num = I2S_PIN_NO_CHANGE,
  .data_in_num = I2S_SD
};

void setup() {
  Serial.begin(115200);
  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  REG_SET_BIT(I2S_TIMING_REG(i2s_num), BIT(9));
  REG_SET_BIT(I2S_CONF_REG(i2s_num), I2S_RX_MSB_SHIFT);
  i2s_set_pin(i2s_num, &pin_config);
  delay(500);

  // Setup the LED channels
  ledcSetup(REDCHANNEL, FREQUENCY, RESOLUTION);
  ledcSetup(GREENCHANNEL, FREQUENCY, RESOLUTION);
  ledcSetup(BLUECHANNEL, FREQUENCY, RESOLUTION);

  ledcAttachPin(RED_PIN, REDCHANNEL);
  ledcAttachPin(GREEN_PIN, GREENCHANNEL);
  ledcAttachPin(BLUE_PIN, BLUECHANNEL);
}

void loop() {
  readAudioData();
  processAudio();
  controlLED();
}

void readAudioData() {
  size_t bytes_read = 0;
  i2s_read(i2s_num, &audio_buf, BUFLEN, &bytes_read, portMAX_DELAY);
}

void processAudio() {
  int32_t cleanBuf[BUFLEN / 2] {0};
  int cleanBufIdx = 0;

  for (int i = 0; i < BUFLEN; i++) {
    if (audio_buf[i] != 0) {
      cleanBuf[cleanBufIdx] = audio_buf[i] >> 14;
      cleanBufIdx++;
    }
  }

  float meanval = 0;
  int volCount = 0;

  for (int i = 0; i < BUFLEN / 2; i++) {
    if (cleanBuf[i] != 0) {
      meanval += cleanBuf[i];
      volCount++;
    }
  }

  meanval /= volCount;

  for (int i = 0; i < volCount; i++) {
    cleanBuf[i] -= meanval;
  }

  float maxsample, minsample;
  minsample = 100000;
  maxsample = -100000;

  for (int i = 0; i < volCount; i++) {
    minsample = min(static_cast<int32_t>(minsample), cleanBuf[i]);
    maxsample = max(maxsample, static_cast<float>(cleanBuf[i]));
  }

  loudness = maxsample - minsample;
}

void controlLED() {
  Serial.print("Loudness : ");
  Serial.println(loudness);

  if (loudness > LOUDNESS_THRESHOLD) {
    setColor(100, 150, 130);
  } else {
    setColor(0, 0, 0);
  }

  delay(50);
}

void setColor(int redValue, int greenValue, int blueValue) {
  ledcWrite(REDCHANNEL, redValue);
  ledcWrite(GREENCHANNEL, greenValue);
  ledcWrite(BLUECHANNEL, blueValue);
}

#include <driver/i2s.h>
#include <Arduino.h>
#include "soc/i2s_reg.h"
#include "pin_configuration.h"

#define BUFLEN 256
int32_t audio_buf[BUFLEN];

float loudness;

const int LOUDNESS_THRESHOLD = 500;

// Relay configuration
const int RELAY_PIN = 13; // Change to the pin you connected the relay to
const int DEBOUNCE_TIME = 500; // Debounce time in milliseconds

bool relayState = false; // Relay state: false = off, true = on
unsigned long lastClapTime = 0; // Last time a clap was detected

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

  // Setup the relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Make sure the relay is off initially
}

void loop() {
  readAudioData();
  processAudio();
  controlRelay();
}

void readAudioData() {
  size_t bytes_read = 0;
  i2s_read(i2s_num, &audio_buf, BUFLEN, &bytes_read, portMAX_DELAY);
}

void processAudio() {
  int32_t cleanBuf[BUFLEN / 2] = {0};
  int cleanBufIdx = 0;

  for (int i = 0; i < BUFLEN; i++) {
    if (audio_buf[i] != 0) {
      cleanBuf[cleanBufIdx] = audio_buf[i] >> 14;
      cleanBufIdx++;
    }
  }

  if (cleanBufIdx == 0) {
    loudness = 0;
    return;
  }

  float meanval = 0;
  int volCount = 0;

  for (int i = 0; i < cleanBufIdx; i++) {
    if (cleanBuf[i] != 0) {
      meanval += cleanBuf[i];
      volCount++;
    }
  }

  if (volCount == 0) {
    loudness = 0;
    return;
  }

  meanval /= volCount;

  for (int i = 0; i < volCount; i++) {
    cleanBuf[i] -= meanval;
  }

  float maxsample = -100000.0;
  float minsample = 100000.0;

  for (int i = 0; i < volCount; i++) {
    minsample = min(minsample, (float)cleanBuf[i]);
    maxsample = max(maxsample, (float)cleanBuf[i]);
  }

  loudness = maxsample - minsample;
}

void controlRelay() {
  Serial.print("Loudness : ");
  Serial.println(loudness);

  unsigned long currentTime = millis();
  if (loudness > LOUDNESS_THRESHOLD && (currentTime - lastClapTime > DEBOUNCE_TIME)) {
    relayState = !relayState;
    digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
    lastClapTime = currentTime;
  }

  delay(50);
}

#include <Arduino.h>
#include "driver/i2s.h"
#include "clip.h"

#define I2S_BCLK  33     // MAX98357A BCLK
#define I2S_LRC   27     // MAX98357A LRC / WS
#define I2S_DOUT  15     // MAX98357A DIN

#define Push_Button A5

#define SAMPLE_RATE 22000

void setup() {
    Serial.begin(9600);
    delay(1000);

    pinMode(Push_Button, INPUT_PULLUP);

    Serial.println("Starting audio....");

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 512,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);

    
}

void loop() {

    Serial.println(digitalRead(Push_Button));

    if(!digitalRead(Push_Button))
    {       delay(500);

            Serial.println("Playing...");

            size_t bytes_written;

            i2s_write(
                I2S_NUM_0,
                clip_raw,
                clip_raw_len,
                &bytes_written,
                portMAX_DELAY
            );

            Serial.println("Done.");

    }

}
/**************************************************
 * ESP32Cam AI-Thinker Transmitter
 * by @hpsaturn Copyright (C) 2024
 * This file is part ESPNowCam project:
 * https://github.com/hpsaturn/ESPNowCam
 **************************************************/

// N O T E:
// -------
// Don't forget first install NanoPb library!
// and also review the README.md file.

#include <Arduino.h>
#include <ESPNowCam.h>
#include <drivers/CamAIThinker.h>

CamAIThinker Camera;
ESPNowCam radio;

void processFrame() {
    if (Camera.get()) {
        uint8_t *out_jpg = NULL;
        size_t out_jpg_len = 0;
        frame2jpg(Camera.fb, 12, &out_jpg, &out_jpg_len);
        radio.sendData(out_jpg, out_jpg_len);
        // Serial.println("camera.get activé");
        free(out_jpg);
        Camera.free();
    }
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    delay(1000); // only for debugging.

    if(psramFound()){
        size_t psram_size = esp_spiram_get_size() / 1048576;
        Serial.printf("PSRAM size: %dMb\r\n", psram_size);
    }
    // TTGO-T1 receiver target (P2P or 1:1 mode)
    // const uint8_t macRecv[6] = {0x08,0xF9,0xE0,0xF4,0x6E,0x14};
    // ESP32-8048S043 receiver target (P2P or 1:1 mode)
    const uint8_t macRecv[6] = {0x34,0xcd,0xb0,0x0d,0xf6,0x88};
    radio.setTarget(macRecv);
    radio.init();

    // You are able to change the Camera config E.g:
    // Camera.config.fb_count = 2;
    // Camera.config.frame_size = FRAMESIZE_QQVGA;

    if (!Camera.begin()) {
        Serial.println("Camera Init Fail");
    }
    delay(500);

    sensor_t * s = esp_camera_sensor_get();
    s->set_brightness(s, 0);     // -2 to 2
    s->set_contrast(s, 0);       // -2 to 2
    s->set_saturation(s, 0);     // -2 to 2
    s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
    s->set_wb_mode(s, 4);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
    s->set_aec2(s, 0);           // 0 = disable , 1 = enable
    s->set_ae_level(s, 0);       // -2 to 2
    s->set_aec_value(s, 300);    // 0 to 1200
    s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0);       // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    s->set_bpc(s, 0);            // 0 = disable , 1 = enable
    s->set_wpc(s, 1);            // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
    s->set_lenc(s, 1);           // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
    s->set_vflip(s, 0);          // 0 = disable , 1 = enable
    s->set_dcw(s, 1);            // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);       // 0 = disable , 1 = enable




}

void loop() {
    processFrame();
}

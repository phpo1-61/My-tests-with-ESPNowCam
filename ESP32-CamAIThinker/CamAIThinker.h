#ifndef CAMTAITHINKER_H
#define CAMTAITHINKER_H

#include "CameraBase.hpp"
#include "esp_camera.h"

class CamAIThinker : public CameraBase {
public:
    using CameraBase::begin;; PlatformIO Project Configuration File
    ;
    ;   Build options: build flags, source filter
    ;   Upload options: custom upload port, speed and extra flags
    ;   Library options: dependencies, extra library storages
    ;   Advanced options: extra scripting
    ;
    ; Please visit documentation for the other options and examples
    ; https://docs.platformio.org/page/projectconf.html

    [env:esp32cam]
    platform = espressif32
    board = esp32cam
    framework = arduino
    monitor_speed = 115200
    monitor_filters =
    esp32_exception_decoder
    time
    build_flags =
    ; -Wall
    ; -Wextra
    ; -Werror
    -D CORE_DEBUG_LEVEL=3
    -D BOARD_HAS_PSRAM
    ;-D CONFIG_ESPTOOLPY_FLASHMODE_QIO  ; mode QIO pour FLASH
    ; -D CONFIG_ESPTOOLPY_FLASHMODE_DIO  ; mode DIO pour FLASH
    lib_deps = hpsaturn/EspNowCam@^0.1.16
    ; extra_scripts = pre:prebuild.py
    upload_speed = 115200

    using CameraBase::free;
    using CameraBase::get;

    CamAIThinker(){
        config.pin_reset = -1;
        config.pin_pwdn = 32;
        config.pin_xclk = 0;
        config.pin_sccb_sda = 26;
        config.pin_sccb_scl = 27;
        config.pin_d7 = 35;
        config.pin_d6 = 34;
        config.pin_d5 = 39;
        config.pin_d4 = 36;
        config.pin_d3 = 21;
        config.pin_d2 = 19;
        config.pin_d1 = 18;
        config.pin_d0 = 5;
        config.pin_vsync = 25;
        config.pin_href = 23;
        config.pin_pclk = 22;
        config.xclk_freq_hz = 16000000;
        config.ledc_timer   = LEDC_TIMER_0;
        config.ledc_channel = LEDC_CHANNEL_0;
        config.pixel_format  = PIXFORMAT_RGB565;
        config.frame_size    = FRAMESIZE_QVGA;
        config.jpeg_quality  = 12;
        config.fb_count      = 2;
        config.fb_location   = CAMERA_FB_IN_PSRAM;
        config.grab_mode     = CAMERA_GRAB_WHEN_EMPTY;
        fb = nullptr;
        sensor = nullptr;
    }
};

#endif

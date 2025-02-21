#include <ESPNowCam.h>
#define CORE_DEBUG_LEVEL = 1
#define LGFX_USE_V1
#include "LovyanGFX.h"

class LGFX : public lgfx::LGFX_Device
{

    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_FT5x06 _touch_instance;

public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host = VSPI_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.freq_read = 14000000;
            cfg.spi_3wire = true;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            cfg.pin_sclk = 18;
            cfg.pin_mosi = 19;
            cfg.pin_miso = -1;
            cfg.pin_dc = 16;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs = 5;
            cfg.pin_rst = 23;
            cfg.pin_busy = -1;
            cfg.panel_width = 135;
            cfg.panel_height = 240;
            cfg.offset_x = 52;
            cfg.offset_y = 40;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 16;
            cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = false;
            _panel_instance.config(cfg);
        }

        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = 4;
            cfg.invert = false;
            cfg.freq = 44100;
            cfg.pwm_channel = 7;
            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
            // test d'affichage sur l'écran
        }
        {
            auto cfg = _touch_instance.config();
            cfg.x_min = 0;
            cfg.x_max = 134;
            cfg.y_min = 0;
            cfg.y_max = 239;
            cfg.pin_int = -1;
            cfg.bus_shared = true;
            cfg.offset_rotation = 0;
            cfg.i2c_port = 1;
            cfg.i2c_addr = 0x38;
            cfg.pin_sda = -1;
            cfg.pin_scl = -1;
            cfg.freq = 400000;
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }
        setPanel(&_panel_instance);
    }
};

#include <LGFX_TFT_eSPI.hpp>
// doit suivre la définition des classes LGFX

ESPNowCam radio;
LGFX tft;

// frame buffer
uint8_t *fb;
// display globals
int32_t dw, dh;

void onDataReady(uint32_t lenght) {
    tft.drawJpg(fb, lenght , 0, 0, dw, dh);
    // printFPS("MF:");
}

void setup() {
    Serial.begin(115200);
    delay (1000);
    Serial.println("setup starts");

    tft.init();
    tft.setRotation(3);
    tft.startWrite();
    // test d'affichage sur l'écran
    //tft.fillScreen(TFT_BLACK);
    delay(1000);
    //tft.fillScreen(TFT_RED);
    //delay(1000);
    //tft.fillScreen(TFT_GREEN);
    //delay(1000);
    //tft.fillScreen(TFT_BLUE);
    //delay(1000);
    tft.fillScreen(TFT_BLACK);
    // delay(1000);
    tft.setCursor(40, 65);
    tft.print("Screen ready to receive");
    //delay (100);
    tft.fillScreen(TFT_BLACK);
    Serial.println("setup ends");

    dw = tft.width();
    dh = tft.height();
    Serial.print("acquired screen width: ");
    Serial.println(dw);
    Serial.print("acquired screen height: ");
    Serial.println(dh);
    if(psramFound()){
        size_t psram_size = esp_spiram_get_size() / 1048576;
        Serial.printf("PSRAM size: %dMb\r\n", psram_size);
    }

    // BE CAREFUL WITH IT, IF JPG LEVEL CHANGES, INCREASE IT
    fb = (uint8_t*)  malloc(15000* sizeof( uint8_t ) ) ;

    radio.setRecvBuffer(fb);
    radio.setRecvCallback(onDataReady);

    if (radio.init()) {
        tft.setTextSize(2);
        tft.setRotation(3);
        tft.drawString("ESPNow Init success", dw / 8, dh / 2);
        tft.setRotation(1);
    }
    delay(1000);
}

void loop() {
}

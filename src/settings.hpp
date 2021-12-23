#pragma once

#include <time.h>

#include <Adafruit_SSD1306.h>

struct Settings
{
  public:
    static Settings &get()
    {
        static Settings s;
        return s;
    }

    bool auto_mode : 1;
    bool light_on  : 1;
    uint8_t max_brightness;
    uint8_t brightness;
    Adafruit_SSD1306 *display;
    uint32_t turn_on_time;
    uint32_t turn_off_time;

  private:
    Settings() = default;
};
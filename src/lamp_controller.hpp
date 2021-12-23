#pragma once

#include <time.h>

#include "settings.hpp"
#include "time.hpp"
#include <Arduino.h>

class LampController
{
  public:
    void run()
    {
        if (Settings::get().auto_mode)
        {
            time_t current_time  = Time::time();
            struct tm *t         = localtime(&current_time);
            time_t turn_on_time  = Settings::get().turn_on_time;
            time_t turn_off_time = Settings::get().turn_off_time;
            struct tm *ton       = localtime(&turn_on_time);
            struct tm *toff      = localtime(&turn_off_time);

            if (t->tm_hour >= ton->tm_hour && t->tm_hour <= toff->tm_hour)
            {
                if (t->tm_min >= ton->tm_min && t->tm_min <= toff->tm_min)
                {
                    Settings::get().light_on = true;
                    analogWrite(PD5, Settings::get().max_brightness);
                    return;
                }
            }
            analogWrite(PD5, 0);
            Settings::get().light_on = false;
        }
        else
        {
            if (Settings::get().brightness)
            {
                Settings::get().light_on = true;
            }
            else
            {
                Settings::get().light_on = false;
            }
            analogWrite(PD5, Settings::get().brightness);
        }
    }
};
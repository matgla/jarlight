#include <stdlib.h>
#include <time.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MemoryUsage.h>
#include <SPI.h>
#include <Wire.h>

#include "buttons.hpp"
#include "iwidget.hpp"
#include "lamp_controller.hpp"
#include "menu.hpp"
#include "settings.hpp"
#include "time.hpp"

static Adafruit_SSD1306 display(128, 64, &Wire, -1);

static Menu menu;

// void TimerHandler1()
// {
//     Time::increment();
// }

static Buttons buttons;

void setup()
{
    Time::init();
    Serial.begin(9600);

    pinMode(PD4, INPUT_PULLUP);
    pinMode(PD3, INPUT_PULLUP);
    pinMode(PD2, INPUT_PULLUP);
    pinMode(PD5, OUTPUT);

    FREERAM_PRINT;

    char buf[4];
    Serial.print(F("sizeof: "));
    itoa(sizeof(Button), buf, 10);
    Serial.println(buf);
    Serial.print(F("sizeof: "));
    itoa(sizeof(Buttons), buf, 10);
    Serial.println(buf);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    struct tm *ts = localtime(&Settings::get().turn_on_time);

    ts->tm_hour = 7;
    ts->tm_min  = 0;

    Settings::get().turn_on_time = mktime(ts);
    struct tm *te                = localtime(&Settings::get().turn_off_time);

    te->tm_hour                   = 21;
    te->tm_min                    = 0;
    Settings::get().turn_off_time = mktime(te);

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    Settings::get().display = &display;
    FREERAM_PRINT;
}

LampController lamp;

void loop()
{
    display.clearDisplay();
    buttons.run();
    menu.process(buttons);
    menu.draw();
    lamp.run();
    display.display();
}

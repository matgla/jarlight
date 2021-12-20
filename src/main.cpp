#include <time.h>
#include <stdlib.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define USE_TIMER_1 true

#include <TimerInterrupt.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

constexpr static uint8_t icon_size = 12;
constexpr static uint8_t text_size = 8;

static const uint8_t PROGMEM sun[] = {
    0x10, 0x80, 0x80, 0x10, 0x4f, 0x20, 0x1f, 0x80, 0xbf, 0xd0, 0x3f, 0xc0, 0x3f, 0xc0, 0xbf, 0xd0,
    0x1f, 0x80, 0x4f, 0x20, 0x80, 0x10, 0x10, 0x80};

static const uint8_t PROGMEM moon[] =
    {
        0x03, 0x80, 0x01, 0xc0, 0x01, 0xe0, 0x00, 0xe0, 0x00, 0xf0, 0x41, 0xf0, 0x73, 0xf0, 0x7f, 0xe0,
        0x7f, 0xe0, 0x3f, 0xc0, 0x0f, 0x00, 0x00, 0x00};

static const uint8_t bulb[] PROGMEM = {
    0x06, 0x00, 0x0f, 0x00, 0x1f, 0x80, 0x3f, 0xc0, 0x3f, 0xc0, 0x3f, 0xc0, 0x1f, 0x80, 0x0f, 0x00,
    0x09, 0x00, 0x09, 0x00, 0x09, 0x00, 0x06, 0x00};
static const uint8_t calendar[] PROGMEM = {
    0x00, 0x00, 0x29, 0x40, 0x3f, 0xc0, 0x69, 0x60, 0x40, 0x20, 0x56, 0xa0, 0x40, 0x20, 0x56, 0xa0,
    0x40, 0x20, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00};

static const uint8_t clock_img[] PROGMEM = {
    0x06, 0x00, 0x19, 0x80, 0x24, 0x40, 0x24, 0x40, 0x44, 0x20, 0x43, 0xa0, 0x20, 0x40, 0x20, 0x40,
    0x19, 0x80, 0x1f, 0x80, 0x39, 0xc0, 0x70, 0xe0};

volatile static uint64_t ticks = 0;
volatile static bool button_l = false;
volatile static bool button_m = false;
volatile static bool button_r = false;

constexpr static uint8_t veritcal_positions[] = {1, 18, 32, 48};

constexpr const char *day_to_text[] = {
    "pon",
    "wt ",
    "sr ",
    "czw",
    "pt ",
    "sob",
    "nie"};

constexpr const char *month_to_text[] = {
    "sty",
    "lut",
    "mar",
    "kwi",
    "maj",
    "cze",
    "lip",
    "sie",
    "wrz",
    "paz",
    "lis",
    "gru"};

constexpr uint8_t text_vertical_offset = (icon_size - text_size) / 2 + 1;

void display_time()
{
    char buf[64];
    time_t epoch = ticks;
    tm t = *localtime(&epoch);
    display.drawBitmap(8, veritcal_positions[1], calendar, 12, 12, 1);
    display.setCursor(8 + icon_size + 4, veritcal_positions[1] + text_vertical_offset - 1);

    snprintf(buf, sizeof(buf), "%s %02d %s %04d", day_to_text[t.tm_mday], t.tm_mday, month_to_text[t.tm_mon + 1], t.tm_year + 1870);
    display.println(buf);

    display.drawBitmap(8, veritcal_positions[2], clock_img, 12, 12, 1);

    display.setCursor(8 + icon_size + 15, veritcal_positions[2] + text_vertical_offset - 1);
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
    display.println(buf);
}

time_t turn_on_time;
time_t turn_off_time;

void draw_time_settings()
{
    char buf[64];
    constexpr uint8_t column_1 = 8;
    constexpr uint8_t column_2 = 74;
    constexpr uint8_t text_row = veritcal_positions[0] + text_vertical_offset;
    display.drawBitmap(column_1, veritcal_positions[0], sun, 12, 12, 1);
    display.drawBitmap(column_2, veritcal_positions[0], moon, 12, 12, 1);
    display.setCursor(column_1 + icon_size + 3, text_row);
    struct tm *ton = localtime(&turn_on_time);
    snprintf(buf, sizeof(buf), "%02d:%02d", ton->tm_hour, ton->tm_min);
    display.println(buf);
    display.setCursor(column_2 + icon_size + 3, text_row);
    struct tm *toff = localtime(&turn_off_time);
    snprintf(buf, sizeof(buf), "%02d:%02d", toff->tm_hour, toff->tm_min);
    display.println(buf);
}

static uint8_t brightness = 80;

void draw_brightness()
{
    display.drawBitmap(8, veritcal_positions[3], bulb, 12, 12, 1);
    char buf[64];
    display.setCursor(icon_size + 8 + 4, veritcal_positions[3] + text_vertical_offset - 1);
    snprintf(buf, sizeof(buf), "%d%%", brightness);
    display.println(buf);
}

void draw_mode()
{
    display.setCursor(64, veritcal_positions[3] + text_vertical_offset - 1);
    char buf[64];
    snprintf(buf, sizeof(buf), "mode: auto");
    display.println(buf);
}

void draw_frame()
{
    draw_time_settings();
    display_time();
    draw_brightness();
    draw_mode();
    display.setCursor(32, 32);
    display.display();
}

void TimerHandler1()
{
    static bool toggle = false;
    digitalWrite(PD5, toggle);
    toggle = !toggle;
    ++ticks;
}

void setup()
{
    Serial.begin(9600);

    pinMode(PD4, INPUT_PULLUP);
    pinMode(PD3, INPUT_PULLUP);
    pinMode(PD2, INPUT_PULLUP);
    pinMode(PD5, OUTPUT);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    struct tm *ts = localtime(&turn_on_time);

    ts->tm_hour = 7;
    ts->tm_min = 0;

    turn_on_time = mktime(ts);
    struct tm *te = localtime(&turn_off_time);

    te->tm_hour = 21;
    te->tm_min = 0;
    turn_off_time = mktime(te);

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.clearDisplay();
    draw_frame();

    ITimer1.init();
    ITimer1.attachInterruptInterval(1000, TimerHandler1);
}

uint8_t i = 0;
void loop()
{
    button_l = digitalRead(PD4);
    button_m = digitalRead(PD3);
    button_r = digitalRead(PD2);

    display.clearDisplay();

    draw_frame();

    if (digitalRead(PD4))
    {
        turn_on_time += 60;
    }
}

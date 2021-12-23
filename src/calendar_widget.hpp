#pragma once

#include <avr/pgmspace.h>
#include <time.h>

#include "time.hpp"

#include "icons.hpp"
#include "menu_item.hpp"

constexpr char monday_string[] PROGMEM    = "pon";
constexpr char tuesday_string[] PROGMEM   = "wt ";
constexpr char wednesday_string[] PROGMEM = "sr ";
constexpr char thursday_string[] PROGMEM  = "czw";
constexpr char friday_string[] PROGMEM    = "pt ";
constexpr char saturday_string[] PROGMEM  = "sob";
constexpr char sunday_string[] PROGMEM    = "nie";

constexpr const char *const day_to_text[] PROGMEM = {
    monday_string, tuesday_string,  wednesday_string, thursday_string,
    friday_string, saturday_string, sunday_string};

constexpr char january_string[] PROGMEM   = "sty";
constexpr char february_string[] PROGMEM  = "lut";
constexpr char march_string[] PROGMEM     = "mar";
constexpr char april_string[] PROGMEM     = "kwi";
constexpr char may_string[] PROGMEM       = "maj";
constexpr char june_string[] PROGMEM      = "cze";
constexpr char july_string[] PROGMEM      = "lip";
constexpr char august_string[] PROGMEM    = "sie";
constexpr char september_string[] PROGMEM = "wrz";
constexpr char october_string[] PROGMEM   = "paz";
constexpr char november_string[] PROGMEM  = "lis";
constexpr char december_string[] PROGMEM  = "gru";

constexpr const char *const month_to_text[] PROGMEM = {
    january_string,   february_string, march_string,    april_string,
    may_string,       june_string,     july_string,     august_string,
    september_string, october_string,  november_string, december_string};

namespace
{

void read_day_from_progmem(char *buf, int day)
{
    strcpy_P(buf, reinterpret_cast<char *>(pgm_read_word(&(day_to_text[day]))));
}

void read_month_from_progmem(char *buf, int month)
{
    strcpy_P(buf, reinterpret_cast<char *>(pgm_read_word(&(month_to_text[month]))));
}

} // namespace

class CalendarWidget : public MenuItem
{
  public:
    CalendarWidget(Position position)
        : MenuItem(position, calendar_icon)
    {
    }

    void set_selected(bool select, bool from_left) override
    {
        MenuItem::set_selected(select, from_left);
    }

    void draw_on_select() const override
    {
    }

    void on_draw(char *buffer, size_t buffer_size) const override
    {
        time_t epoch = Time::time();
        tm t         = *localtime(&epoch);

        char day[4];
        char month[4];

        read_day_from_progmem(day, t.tm_mday - 1);
        read_month_from_progmem(month, t.tm_mon);

        snprintf(buffer, buffer_size, "%s %2d %s %4d", day, t.tm_mday, month, t.tm_year + 1870);
    }

    bool click(Buttons &buttons) override
    {
        return false;
    }

    bool is_select_able() const override
    {
        return false;
    }
};

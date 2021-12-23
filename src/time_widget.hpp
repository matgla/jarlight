#pragma once

#include <time.h>

#include "icons.hpp"
#include "menu_item.hpp"
#include "position.hpp"

class TimeWidget : public MenuItem
{
  public:
    TimeWidget(Position position, const uint8_t *icon, time_t &set_time)
        : MenuItem(position, icon)
        , time_(set_time)
        , edit_minutes_(false)
    {
    }

    void draw_on_select() const override
    {
        if (!edit_minutes_)
        {
            Settings::get().display->drawFastHLine(position_.x + icon_size + 2,
                                                   position_.y + icon_size + 1, 16, 1);
        }
        else
        {
            Settings::get().display->drawFastHLine(position_.x + icon_size * 2 + 2,
                                                   position_.y + icon_size + 1, 16, 1);
        }
    }

    void set_selected(bool select, bool from_left) override
    {
        if (from_left)
        {
            edit_minutes_ = false;
        }
        else
        {
            edit_minutes_ = true;
        }

        MenuItem::set_selected(select, from_left);
    }

    void on_draw(char *buffer, size_t buffer_size) const override
    {
        struct tm *t = localtime(&time_);
        if (is_active())
        {
            if (edit_minutes_)
            {
                Settings::get().display->drawRect(position_.x + icon_size * 2 + 8, position_.y, 15,
                                                  11, 1);
            }
            else
            {
                Settings::get().display->drawRect(position_.x + icon_size + 2, position_.y, 15, 11,
                                                  1);
            }
        }
        snprintf(buffer, buffer_size, "%02d:%02d", t->tm_hour, t->tm_min);
    }

    bool click(Buttons &buttons) override
    {
        if (process_active(buttons))
        {
            return true;
        }

        if (is_active())
        {
            if (process_time_change(buttons))
            {
                return true;
            }
        }

        if (buttons.read(Buttons::Position::Left) == Buttons::Status::ToggleOff)
        {
            if (edit_minutes_)
            {
                edit_minutes_ = false;
                return true;
            }
            return false;
        }

        if (buttons.read(Buttons::Position::Right) == Buttons::Status::ToggleOff)
        {
            if (!edit_minutes_)
            {
                edit_minutes_ = true;
                return true;
            }
            return false;
        }

        return is_active();
    }

    bool is_select_able() const override
    {

        return true;
    }

  private:
    void decrement_time()
    {
        struct tm *t = localtime(&time_);

        if (!edit_minutes_)
        {
            --t->tm_hour;
        }
        else
        {
            if (t->tm_min == 0)
            {
                t->tm_min = 59;
            }
            else
            {
                --t->tm_min;
            }
        }
        time_ = mktime(t);
    }

    void increment_time()
    {
        struct tm *t = localtime(&time_);

        if (!edit_minutes_)
        {
            ++t->tm_hour;
        }
        else
        {
            if (t->tm_min == 59)
            {
                t->tm_min = 0;
            }
            else
            {
                ++t->tm_min;
            }
        }
        time_ = mktime(t);
    }

    bool process_time_change(Buttons &buttons)
    {
        if (buttons.read(Buttons::Position::Left) == Buttons::Status::ToggleOff ||
            buttons.read(Buttons::Position::Left) == Buttons::Status::HoldOn)
        {
            decrement_time();
        }

        if (buttons.read(Buttons::Position::Right) == Buttons::Status::ToggleOff ||
            buttons.read(Buttons::Position::Right) == Buttons::Status::HoldOn)
        {
            increment_time();
        }

        if (buttons.read(Buttons::Position::Mid) == Buttons::Status::ToggleOff)
        {
            return false;
        }

        return true;
    }

    time_t &time_;
    bool edit_minutes_;
};

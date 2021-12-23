#pragma once

#include <time.h>

#include "time.hpp"

#include "icons.hpp"
#include "menu_item.hpp"

class ClockWidget : public MenuItem
{
  public:
    ClockWidget(Position position)
        : MenuItem(position, clock_icon)
        , state_(State::EditHours)
    {
    }

    void set_selected(bool select, bool from_left) override
    {
        MenuItem::set_selected(select, from_left);
    }

    void draw_on_select() const override
    {
        switch (state_)
        {
        case State::EditHours: {
            Settings::get().display->drawFastHLine(position_.x + icon_size + 2,
                                                   position_.y + icon_size + 1, 16, 1);
        }
        break;
        case State::EditMinutes: {
            Settings::get().display->drawFastHLine(position_.x + (icon_size + 2) * 2,
                                                   position_.y + icon_size + 1, 16, 1);
        }
        break;
        case State::EditSeconds: {
            Settings::get().display->drawFastHLine(position_.x + (icon_size + 2) * 3,
                                                   position_.y + icon_size + 1, 16, 1);
        }
        break;
        }
    }

    void on_draw(char *buffer, size_t buffer_size) const override
    {
        time_t tm    = Time::time();
        struct tm *t = localtime(&tm);
        snprintf(buffer, buffer_size, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
        if (is_active())
        {
            switch (state_)
            {
            case State::EditHours: {
                Settings::get().display->drawRect(position_.x + icon_size + 8, position_.y, 15, 11,
                                                  1);
            }
            break;
            case State::EditMinutes: {
                Settings::get().display->drawRect(position_.x + (icon_size + 2) * 2, position_.y,
                                                  15, 11, 1);
            }
            break;
            case State::EditSeconds: {
                Settings::get().display->drawRect(position_.x + (icon_size + 2) * 2, position_.y,
                                                  15, 11, 1);
            }
            }
        }
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
            return process_state(false);
        }

        if (buttons.read(Buttons::Position::Right) == Buttons::Status::ToggleOff)
        {
            return process_state(true);
        }

        return false;
    }

    bool is_select_able() const override
    {
        return true;
    }

  protected:
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

    bool process_state(bool to_right)
    {
        switch (state_)
        {
        case State::EditHours: {
            if (to_right)
            {
                state_ = State::EditMinutes;
                return true;
            }
            return false;
        }
        case State::EditMinutes: {
            if (to_right)
            {
                state_ = State::EditSeconds;
                return true;
            }
            state_ = State::EditHours;
            return true;
        }
        case State::EditSeconds: {
            if (to_right)
            {
                return false;
            }
            state_ = State::EditMinutes;
            return true;
        }
        }
        return false;
    }

    void decrement_time()
    {
        time_t tt    = Time::time();
        struct tm *t = localtime(&tt);

        switch (state_)
        {
        case State::EditHours: {
            --t->tm_hour;
        }
        break;
        case State::EditMinutes: {
            if (t->tm_min == 0)
            {
                t->tm_min = 59;
            }
            else
            {
                --t->tm_min;
            }
        }
        break;
        case State::EditSeconds: {
            if (t->tm_sec == 0)
            {
                t->tm_sec = 59;
            }
            else
            {
                --t->tm_sec;
            }
        }
        break;
        }
        Time::set_time(mktime(t));
    }

    void increment_time()
    {
        time_t tt    = Time::time();
        struct tm *t = localtime(&tt);

        switch (state_)
        {
        case State::EditHours: {
            ++t->tm_hour;
        }
        break;
        case State::EditMinutes: {
            if (t->tm_min == 59)
            {
                t->tm_min = 0;
            }
            else
            {
                ++t->tm_min;
            }
        }
        break;
        case State::EditSeconds: {
            if (t->tm_sec == 59)
            {
                t->tm_sec = 0;
            }
            else
            {
                ++t->tm_sec;
            }
        }
        break;
        }
        Time::set_time(mktime(t));
    }

    enum class State : uint8_t
    {
        EditHours,
        EditMinutes,
        EditSeconds
    };

    State state_;
};
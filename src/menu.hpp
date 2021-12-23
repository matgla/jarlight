#pragma once

#include <time.h>

#include "brightness_widget.hpp"
#include "buttons.hpp"
#include "calendar_widget.hpp"
#include "clock_widget.hpp"
#include "icons.hpp"
#include "mode_widget.hpp"
#include "settings.hpp"
#include "time_widget.hpp"

class Menu
{
  public:
    Menu()
        : turn_on_time_widget({.y = 1, .x = 4}, sun_icon, Settings::get().turn_on_time)
        , turn_off_time_widget({.y = 1, .x = 72}, moon_icon, Settings::get().turn_off_time)
        , clock_widget({.y = 15, .x = 4})
        , brightness_widget({.y = 43, .x = 4})
        , mode_widget({.y = 43, .x = 64})
        , current_widget_id(0)
    {
    }

    void draw()
    {
        turn_on_time_widget.draw();
        turn_off_time_widget.draw();
        clock_widget.draw();
        brightness_widget.draw();
        mode_widget.draw();
    }

    void process(Buttons &buttons)
    {
        if (process_widget(buttons))
        {
            return;
        }

        if (buttons.read(Buttons::Position::Left) == Buttons::Status::ToggleOff)
        {
            unselect_current_widget(true);
            select_previous_widget();
        }

        if (buttons.read(Buttons::Position::Right) == Buttons::Status::ToggleOff)
        {
            unselect_current_widget(true);
            select_next_widget();
        }
    }

  private:
    bool process_widget(Buttons &buttons)
    {
        auto *current = current_widget();
        if (current_widget_id != 0)
        {
            if (current->is_selected())
            {
                return current->click(buttons);
            }
        }
        return false;
    }

    IWidget *current_widget()
    {
        switch (current_widget_id)
        {
        case 0:
            return nullptr;
        case 1:
            return &turn_on_time_widget;
        case 2:
            return &turn_off_time_widget;
        case 3:
            return &clock_widget;
        case 4:
            return &brightness_widget;
        case 5:
            return &mode_widget;
        }
        return nullptr;
    }

    void unselect_current_widget(bool from_left)
    {
        auto *current = current_widget();
        if (current)
        {
            current->set_selected(false, from_left);
        }
    }

    void select_current_widget(bool from_left)
    {
        auto *current = current_widget();
        if (current)
        {
            current->set_selected(true, from_left);
        }
    }

    void select_next_widget()
    {
        if (current_widget_id < 5)
        {
            ++current_widget_id;
        }
        else
        {
            current_widget_id = 0;
        }

        select_current_widget(true);

        auto *current = current_widget();
        if (current && !current->is_select_able())
        {
            select_next_widget();
        }
    }

    void select_previous_widget()
    {
        if (current_widget_id == 0)
        {
            current_widget_id = 5;
        }
        else
        {
            --current_widget_id;
        }

        select_current_widget(false);

        auto *current = current_widget();
        if (current && !current->is_select_able())
        {
            select_next_widget();
        }
    }

    TimeWidget turn_on_time_widget;
    TimeWidget turn_off_time_widget;
    ClockWidget clock_widget;
    BrightnessWidget brightness_widget;
    ModeWidget mode_widget;
    uint8_t current_widget_id;
};

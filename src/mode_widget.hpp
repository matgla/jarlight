#pragma once

#include <avr/pgmspace.h>

#include "icons.hpp"
#include "menu_item.hpp"
#include "settings.hpp"

class ModeWidget : public MenuItem
{
  public:
    ModeWidget(Position position)
        : MenuItem(position, nullptr)
    {
    }

    void set_selected(bool select, bool from_left) override
    {
        MenuItem::set_selected(select, from_left);
    }

    void draw_on_select() const override
    {
        Settings::get().display->drawFastHLine(position_.x + icon_size + 2,
                                               position_.y + icon_size + 1, 16, 1);
    }

    void on_draw(char *buffer, size_t buffer_size) const override
    {
        char mode = 'a';
        if (!Settings::get().auto_mode)
        {
            mode = 'm';
        }
        snprintf(buffer, buffer_size, "m: %c l: %d", mode, Settings::get().light_on);
    }

    bool click(Buttons &buttons) override
    {
        if (buttons.read(Buttons::Position::Mid) == Buttons::Status::ToggleOff ||
            buttons.read(Buttons::Position::Mid) == Buttons::Status::HoldOn)
        {
            Settings::get().auto_mode = !Settings::get().auto_mode;
        }
        return false;
    }

    bool is_select_able() const override
    {
        return true;
    }
};

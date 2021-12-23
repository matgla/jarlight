#pragma once

#include "icons.hpp"
#include "menu_item.hpp"
#include "settings.hpp"

class BrightnessWidget : public MenuItem
{
  public:
    BrightnessWidget(Position position)
        : MenuItem(position, bulb_icon)
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
        uint8_t brightness = 0;
        if (Settings::get().auto_mode)
        {
            brightness = Settings::get().max_brightness;
        }
        else
        {
            brightness = Settings::get().brightness;
        }

        uint16_t percent = brightness * 100 / 255;
        snprintf(buffer, buffer_size, "%3d %%", percent);

        if (is_active())
        {
            Settings::get().display->drawRect(position_.x + icon_size * 2 + 8, position_.y, 15, 11,
                                              1);
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
            if (process_brightness_change(buttons))
            {
                return true;
            }
        }

        return is_active();
    }

    bool is_select_able() const override
    {
        return true;
    }

  protected:
    bool process_brightness_change(Buttons &buttons)
    {
        if (buttons.read(Buttons::Position::Left) == Buttons::Status::ToggleOff ||
            buttons.read(Buttons::Position::Left) == Buttons::Status::HoldOn)
        {
            if (!Settings::get().auto_mode)
            {
                --Settings::get().brightness;
            }
            else
            {
                --Settings::get().max_brightness;
            }
        }

        if (buttons.read(Buttons::Position::Right) == Buttons::Status::ToggleOff ||
            buttons.read(Buttons::Position::Right) == Buttons::Status::HoldOn)
        {
            if (!Settings::get().auto_mode)
            {
                ++Settings::get().brightness;
            }
            else
            {
                ++Settings::get().max_brightness;
            }
        }

        if (buttons.read(Buttons::Position::Mid) == Buttons::Status::ToggleOff)
        {
            return false;
        }

        return true;
    }
};

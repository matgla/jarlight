#pragma once

#include <Adafruit_SSD1306.h>

#include "buttons.hpp"
#include "iwidget.hpp"
#include "position.hpp"

constexpr static uint8_t text_size     = 8;
constexpr uint8_t text_vertical_offset = (icon_size - text_size) / 2 + 1;

struct MenuItem : public IWidget
{
  public:
    MenuItem(Position position, const uint8_t *icon)
        : is_selected_(false)
        , is_active_(false)
        , position_(position)
        , icon_(icon)
    {
    }

    virtual ~MenuItem() = default;

    virtual void on_draw(char *, size_t) const = 0;

    void draw() const override
    {
        char buf[20] = {0};
        Position pos = {position_.y, position_.x};
        if (icon_)
        {
            Settings::get().display->drawBitmap(pos.x, pos.y, icon_, icon_size, icon_size, 1);
            pos.x += icon_size + 4;
            pos.y += text_vertical_offset - 1;
        }

        Settings::get().display->setCursor(pos.x, pos.y);
        on_draw(buf, sizeof(buf));
        Settings::get().display->println(buf);

        if (is_selected_)
        {
            draw_on_select();
        }
    }

    void set_selected(bool select, bool from_left) override
    {
        is_selected_ = select;
    }

    void set_active(bool active) override
    {
        is_active_ = active;
    }

    bool is_active() const override
    {
        return is_active_;
    }

    bool is_selected() const override
    {
        return is_selected_;
    }

  protected:
    bool process_active(Buttons &buttons)
    {
        if (buttons.read(Buttons::Position::Mid) == Buttons::Status::ToggleOff)
        {
            if (!is_active())
            {
                set_active(true);
            }
            else
            {
                set_active(false);
            }
            return true;
        }
        return false;
    }

    bool is_selected_   : 1;
    bool is_active_     : 1;
    bool is_selectable_ : 1;
    Position position_;
    const uint8_t *icon_;
};

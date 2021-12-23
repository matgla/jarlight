#pragma once

#include <Arduino.h>
#include <Button.h>

struct Buttons
{
  public:
    Buttons()
        : button_left(PD4, 2)
        , button_mid(PD3, 2)
        , button_right(PD2, 2)
    {
        button_left.begin();
        button_mid.begin();
        button_right.begin();
    }

    enum class Position : uint8_t
    {
        Left,
        Mid,
        Right
    };

    enum class Status : uint8_t
    {
        ToggleOn,
        ToggleOff,
        On,
        HoldOn,
        Off
    };

    void run()
    {
        status[0] = read_button(button_left, left_hold_time);
        status[1] = read_button(button_mid, mid_hold_time);
        status[2] = read_button(button_right, right_hold_time);
    }

    Status read(Position pos)
    {
        switch (pos)
        {
        case Position::Left: {
            return status[0];
        }
        break;
        case Position::Mid: {
            return status[1];
        }
        break;
        case Position::Right: {
            return status[2];
        }
        break;
        }
        return Status::Off;
    }

  private:
    Status read_button(Button &button, uint8_t &hold)
    {
        auto button_state = button.read();
        if (button.has_changed())
        {
            if (button_state == Button::PRESSED)
            {
                hold = 0;
                return Status::ToggleOn;
            }
            else
            {
                return Status::ToggleOff;
            }
        }

        ++hold;
        if (button_state == Button::PRESSED && (hold > 20))
        {
            return Status::HoldOn;
        }

        return Status::Off;
    }

    uint8_t left_hold_time;
    uint8_t mid_hold_time;
    uint8_t right_hold_time;
    Status status[3];

    Button button_left;
    Button button_mid;
    Button button_right;
};

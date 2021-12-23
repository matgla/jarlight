#pragma once

#include "buttons.hpp"

class IWidget
{
  public:
    virtual ~IWidget()                                     = default;
    virtual void draw() const                              = 0;
    virtual void set_active(bool active)                   = 0;
    virtual bool is_active() const                         = 0;
    virtual void set_selected(bool select, bool from_left) = 0;
    virtual bool is_selected() const                       = 0;
    virtual bool click(Buttons &buttons)                   = 0;
    virtual bool is_select_able() const                    = 0;
    virtual void draw_on_select() const                    = 0;
};
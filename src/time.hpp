#pragma once

#include <time.h>

class Time
{
  public:
    static void init();
    static time_t time();
    static void increment();
    static void set_time(time_t time);

  private:
    volatile static time_t time_;
};

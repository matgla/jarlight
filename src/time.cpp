#include "time.hpp"

#include <Arduino.h>

void Time::init()
{
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A  = 15624;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    Time::increment();
}

void Time::set_time(time_t time)
{
    time_ = time;
}

time_t Time::time()
{
    return time_;
}

void Time::increment()
{
    ++time_;
}

volatile time_t Time::time_ = 0;

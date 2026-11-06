#ifndef TIMER_H
#define TIMER_H

static inline void wait(unsigned int milliseconds)
{
    for (volatile unsigned int i = 0; i < milliseconds * 10000; i++)
    {
        __asm__ volatile ("nop");
    }
}

#endif
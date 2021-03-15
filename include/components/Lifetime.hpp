#include <stdint.h>

struct C_Lifetime
{
    C_Lifetime(uint32_t timer_) : timer(timer_) {}
    uint32_t timer = 0;
};
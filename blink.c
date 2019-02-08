#include "blink.h"
#include "primes.h"


// 8 MHz clock
// 256 Prescaling
uint16_t blink(void) {

    initClk();

    uint16_t nextStateChange = CLKPERIOD;

    LCDDR0 |= 0x40; // init state
    
    while (1) {

        while (TCNT1 < nextStateChange); // Busy wait

        LCDDR0 ^= 0x40; // Flip the bit

        nextStateChange += CLKPERIOD;

        while (TCNT1 > nextStateChange); // Busy wait if and until wraparound happens
    }

    return 0;
}

void initClk(void) {
    // Timer/Counter1 Control Register B
    TCCR1B = (1<<CS12); // System clk with 256 prescaler factor
}


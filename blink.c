#include "blink.h"
#include "primes.h"

#define CLKPERIOD 15625 // (8 M / 256) / 2

// 8 MHz clock
// 256 Prescaling
uint16_t blink(void) {

    initClk();

    uint16_t nextStateChange = CLKPERIOD;

    LCDDR0 = (LCDDR0 & 0xFF) | 0x40; // init state
    
    while (1) {

        while (TCNT1 < nextStateChange); // Busy wait

        LCDDR0 ^= 0x40; // Flip the bit

        nextStateChange += CLKPERIOD;
        //writeLong((long) nextStateChange);

        while (TCNT1 > nextStateChange);


//      diff = 0xFFFF - TCNT1;

//      if (diff < CLKPERIOD) { // Clock is about to wrap around
//          nextStateChange = CLKPERIOD - diff;
//          writeLong((long) nextStateChange);
//          while (nextStateChange < TCNT1) {} // Busy wait until clock wraps around
//      }
//      else {
//          nextStateChange += CLKPERIOD;
//          writeLong((long) nextStateChange);
//      }
    }

    return 0;
}

void initClk(void) {
    // Timer/Counter1 Control Register B
    TCCR1B = (1<<CS12); // System clk with 256 prescaler factor
}


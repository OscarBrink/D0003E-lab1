#include "blink.h"


uint16_t blink(void) {

    initClk();
    
    while (1) {
        /* 0x7FFF = 0xFFFF>>1 */
        LCDDR0 = (LCDDR0 & 0xFF) | 0x40;
        while (TCNT1 < 0x7FFF) {
            // Busy wait
        }

        LCDDR0 = (LCDDR0 & 0xBF);
        while (TCNT1 >= 0x7FFF) {
            // Busy wait
        }
    }


    return 0;
}

void initClk(void) {
    // Timer/Counter1 Control Register B
    TCCR1B = (1<<CS12); // System clk with 256 prescaler factor
}


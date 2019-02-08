#include "button.h"
#include "primes.h"


uint16_t button(void) {

    LCDDR1 = 0x20; // Set initial state

    initIO();

    while (1) {
        while (PINB & (1<<PINB7)); // busy wait until input
        changeState();
        while ( !(PINB & (1<<PINB7)) ); // busy wait until joystick is released
    }

    return 0;
}

void changeState(void) {
    LCDDR1 = (LCDDR1 ^ 0x60);
}


void initIO(void) {
    // "Activate" down input from joystick (bit 7 in PORTB)
    PORTB = (1<<PB7);
}


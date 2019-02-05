#include "button.h"


uint16_t button(void) {

    LCDDR1 = 0x20; // Set initial state
    
    initIO();

    uint8_t current = ( PINB & (1<<PINB7) );

    while (1) {
        while (current == ( PINB & (1<<PINB7) )) {
            // busy wait
        }
        current = ( PINB & (1<<PINB7) );
        changeState();
    }

    return 0;
}

void changeState(void) {
    LCDDR1 = (LCDDR1 ^ 0x60);
}


void initIO(void) {
    PORTB = (1<<PB7);
}


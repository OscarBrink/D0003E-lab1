#include "include/avr/io.h"
#include <stdint.h>
#include "primes.h"
#include "blink.h"
#include "button.h"

// Function Prototypes
void initLCD(void);
void runAll(uint16_t primeStart);
void changeBlink(uint16_t *blinkState, uint16_t *nextStateChange);
void changeButton(uint8_t *buttonState);


int main(void) {

    // Disable CPU clock prescaler
    // Bit 7 of CLKPR is CLKPCE (Clock Prescaler Change Enable)
    CLKPR = 0x80;
    CLKPR = 0x00;

    initLCD();

    /* Part 1 */
    //primes();
    

    /* Part 2 */
    //blink();


    /* Part 3 */
    //button();


    /* Part 4 */
    runAll(3000);


    return 0;
}


void runAll(uint16_t primeStart) {

    initClk();
    initIO();

    uint16_t blinkState = 0;
    uint16_t nextStateChange = CLKPERIOD;
    LCDDR0 |= 0x40; // init blink state

    uint8_t buttonState = 0; // init button state
    LCDDR1 = 0x20; // init button event

    long p = primeStart; // initial prime

    while (1) {

        changeBlink(&blinkState, &nextStateChange);

        changeButton(&buttonState);


        /* Check next number */
        if (isPrime(p)) {
            writeLong(p);
        }
        p++;
    }
}

/* Check if time to change blink state */
void changeBlink(uint16_t *blinkState, uint16_t *nextStateChange) {

    if ( TCNT1 >= (*nextStateChange) && !(*blinkState) ) {
        LCDDR0 ^= 0x40; // Flip the bit
        (*nextStateChange) += CLKPERIOD;

        if ( TCNT1 >= (*nextStateChange) ) { // If wraparound is about to happen
            (*blinkState) = 1;
        }
    }
    else if ( TCNT1 < (*nextStateChange) && (*blinkState) ) {
        (*blinkState) = 0;
    }
}


/* Check if input has happened */
void changeButton(uint8_t *buttonState) {

    if ( !(PINB & (1<<PINB7)) && !(*buttonState) ) {
        LCDDR1 = (LCDDR1 ^ 0x60);
        (*buttonState) = 1;
    } 
    else if ( PINB & (1<<PINB7) ) {
        (*buttonState) = 0;
    }
}


void initLCD(void) {

    // LCD Control Register B
    LCDCRB = (1<<LCDCS)                             // Async clock select
        | (1<<LCDMUX1) | (1<<LCDMUX0)               // 1/3 bias
        | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);  // Port pins to use as segment drivers (25)

    // LCD Frame Rate Register
    // LCD Prescaler Select (3 bits): D = 16
    LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0); // LCD Clock Divide = 8

    // LCD Contrast Control
    // LCD Display Configuration (3 bits): 300 us Nominal drive time
    LCDCCR =  (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0); // LCD Max voltage = 3.35 V

    // LCD Control and Status
    LCDCRA = (1<<LCDEN) // LCD Enable
        | (1<<LCDAB);   // LCD Low Power Waveform
}


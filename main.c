#include "include/avr/io.h"
#include <stdint.h>
#include "primes.h"
#include "blink.h"
#include "button.h"

// Function Prototypes
void initLCD(void);
void runAll(void);

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
    runAll();


    return 0;
}


void runAll(void) {

    initClk();
    initIO();

    uint16_t blink_state = 0;
    LCDDR0 = (LCDDR0 & 0xFF) | 0x40; // init blink state

    uint8_t button_state = ( PINB & (1<<PINB7) ); // init button state
    LCDDR1 = 0x20; // init button event

    long p = 2; // initial prime

    while (1) {
        /* Check if time to change blink state */
        if (!blink_state && TCNT1 >= 0x7FFF) {
            LCDDR0 = (LCDDR0 & 0xBF);
            blink_state = 1;
        }
        else if (blink_state && TCNT1 < 0x7FFF) {
            LCDDR0 = (LCDDR0 & 0xFF) | 0x40;
            blink_state = 0;
        }

        /* Check if input has happened */
        uint16_t new_button_state = (PINB & (1<<PINB7));
        if (button_state != new_button_state) {
            button_state = new_button_state;
            LCDDR1 = (LCDDR1 ^ 0x60);
        }

        /* Check next number */
        if (isPrime(p)) {
            writeLong(p);
        }
        p++;
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


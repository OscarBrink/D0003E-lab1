#include "include/avr/io.h"
#include <stdint.h>

// Function Prototypes
void initLCD(void);
uint16_t writeChar(char ch, int pos);
uint16_t mapLCDSegments(uint8_t *segmentMap, char ch);

uint16_t writeLong(long i);

uint16_t primes(void);
uint16_t isPrime(long i);


int main(void) {

    // Disable CPU clock prescaler
    // Bit 7 of CLKPR is CLKPCE (Clock Prescaler Change Enable)
    CLKPR = 0x80;
    CLKPR = 0x00;

    initLCD();

    primes();

    return 0;
}

uint16_t primes(void) {

    long p = 2;
    while (1) {
        if (isPrime(p)) {
            writeLong(p);
        }
        p++;
    }

    return 0;
}

uint16_t isPrime(long i) {

    if (i == 2) {
        return 1;
    }

    for (long j = 2; j < (i >> 1); j++) {
        if (i % j == 0) {
            return 0;
        }
    }
    return 1;
}

// Write positive long
uint16_t writeLong(long i) {

    uint16_t pos = 5; // Write LSD first

    if (i == 0) {
        if (writeChar('0', pos)) {
            return 1;
        }
    }
    else {
        for (; i > 0; i /= 10) {
            if (writeChar('0' + i % 10, pos--)) {
                return 1;
            }
        }
    }

    return 0;
}

uint16_t writeChar(char ch, int pos) {

    if (pos > 5) {
        return 1;
    }

    if (ch < 48 || ch > 57) {
        return 1;
    }
    
    // Determine LCD-segments to activate
    uint8_t segmentMap[4];
    if (mapLCDSegments(segmentMap, ch)) { // Error check
        return 1;
    }

    uint16_t offset = ( (pos & 1) == 1 ? 4 : 0); // Odd numbers on upper nibble

    /*
     * Clear desired nibble and write value to it
     */
    if (pos == 0 || pos == 1) {
        LCDDR0  = ( LCDDR0  & (0xF0 >> offset) ) | (segmentMap[0] << offset);
        LCDDR5  = ( LCDDR5  & (0xF0 >> offset) ) | (segmentMap[1] << offset);
        LCDDR10 = ( LCDDR10 & (0xF0 >> offset) ) | (segmentMap[2] << offset);
        LCDDR15 = ( LCDDR15 & (0xF0 >> offset) ) | (segmentMap[3] << offset);
    }
    else if (pos == 2 || pos == 3) {
        LCDDR1  = ( LCDDR1  & (0xF0 >> offset) ) | (segmentMap[0] << offset);
        LCDDR6  = ( LCDDR6  & (0xF0 >> offset) ) | (segmentMap[1] << offset);
        LCDDR11 = ( LCDDR11 & (0xF0 >> offset) ) | (segmentMap[2] << offset);
        LCDDR16 = ( LCDDR16 & (0xF0 >> offset) ) | (segmentMap[3] << offset);
    }
    else if (pos == 4 || pos == 5) {
        LCDDR2  = ( LCDDR2  & (0xF0 >> offset) ) | (segmentMap[0] << offset);
        LCDDR7  = ( LCDDR7  & (0xF0 >> offset) ) | (segmentMap[1] << offset);
        LCDDR12 = ( LCDDR12 & (0xF0 >> offset) ) | (segmentMap[2] << offset);
        LCDDR17 = ( LCDDR17 & (0xF0 >> offset) ) | (segmentMap[3] << offset);
    }

    return 0;

}

uint16_t mapLCDSegments(uint8_t *segmentMap, char ch) {

    /*
     * Maps the ASCII-characters to the segments in the LCD.
     */
    switch (ch) {
        case '0':
            segmentMap[0] = 0b1001;
            segmentMap[1] = 0b0101;
            segmentMap[2] = 0b0101;
            segmentMap[3] = 0b0101;
            break;
        case '1':
            segmentMap[0] = 0b0000;
            segmentMap[1] = 0b0001;
            segmentMap[2] = 0b0001;
            segmentMap[3] = 0b0000;
            break;
        case '2':
            segmentMap[0] = 0b0001;
            segmentMap[1] = 0b0001;
            segmentMap[2] = 0b1110;
            segmentMap[3] = 0b0001;
            break;
        case '3':
            segmentMap[0] = 0b0001;
            segmentMap[1] = 0b0001;
            segmentMap[2] = 0b1011;
            segmentMap[3] = 0b0001;
            break;
        case '4':
            segmentMap[0] = 0b0000;
            segmentMap[1] = 0b0101;
            segmentMap[2] = 0b1011;
            segmentMap[3] = 0b0000;
            break;
        case '5':
            segmentMap[0] = 0b0001;
            segmentMap[1] = 0b0100;
            segmentMap[2] = 0b1011;
            segmentMap[3] = 0b0001;
            break;
        case '6':
            segmentMap[0] = 0b0001;
            segmentMap[1] = 0b0100;
            segmentMap[2] = 0b1111;
            segmentMap[3] = 0b0001;
            break;
        case '7':
            segmentMap[0] = 0b0001;
            segmentMap[1] = 0b0001;
            segmentMap[2] = 0b0001;
            segmentMap[3] = 0b0000;
            break;
        case '8':
            segmentMap[0] = 0b0001;
            segmentMap[1] = 0b0101;
            segmentMap[2] = 0b1111;
            segmentMap[3] = 0b0001;
            break;
        case '9':
            segmentMap[0] = 0b0001;
            segmentMap[1] = 0b0101;
            segmentMap[2] = 0b1011;
            segmentMap[3] = 0b0001;
            break;
        default:
            // Err
            return 1;
    }
    return 0;
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


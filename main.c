#include "include/avr/io.h"
#include <stdint.h>

void initLCD(void);
void writeChar(char ch, int pos);
void mapLCDSegments(uint8_t *segmentMap, char ch);

int main(void) {

    // Disable CPU clock prescaler
    // Bit 7 of CLKPR is CLKPCE (Clock Prescaler Change Enable)
    CLKPR = 0x80;
    CLKPR = 0x00;

    initLCD();

    // LCD Data Registers
    // LCDDR1 = 0xff;

//  LCDDR0 = 0x10;
//  LCDDR5 = 0x55;
//  LCDDR10 = 0xFF;
//  LCDDR15 = 0x11;
//
//  LCDDR1 = 0x11;
//  LCDDR6 = 0x55;
//  LCDDR11 = 0xFF;
//  LCDDR16 = 0x11;
//
//  LCDDR2 = 0x11;
//  LCDDR7 = 0x55;
//  LCDDR12 = 0xFF;
//  LCDDR17 = 0x11;

    writeChar('0', 0);
    writeChar('6', 1);
    writeChar('7', 2);

    writeChar('a', 0);

    writeChar('8', 3);
    writeChar('9', 4);
    writeChar('5', 5);

    writeChar('8', 6);

    return 0;
}

void writeChar(char ch, int pos) {

    if (pos > 5) {
        return;
    }

    if (ch < 48 || ch > 57) {
        return;
    }
    
    // Determine LCD-segments to activate
    uint8_t segmentMap[4];
    mapLCDSegments(segmentMap, ch);

    uint16_t offset = (pos & 1 == 1 ? 4 : 0);

    if (pos == 0 || pos == 1) {
        LCDDR0  = LCDDR0  | (segmentMap[0] << offset);
        LCDDR5  = LCDDR5  | (segmentMap[1] << offset);
        LCDDR10 = LCDDR10 | (segmentMap[2] << offset);
        LCDDR15 = LCDDR15 | (segmentMap[3] << offset);
    }
    else if (pos == 2 || pos == 3) {
        LCDDR1  = LCDDR1  | (segmentMap[0] << offset);
        LCDDR6  = LCDDR6  | (segmentMap[1] << offset);
        LCDDR11 = LCDDR11 | (segmentMap[2] << offset);
        LCDDR16 = LCDDR16 | (segmentMap[3] << offset);
    }
    else if (pos == 4 || pos == 5) {
        LCDDR2  = LCDDR2  | (segmentMap[0] << offset);
        LCDDR7  = LCDDR7  | (segmentMap[1] << offset);
        LCDDR12 = LCDDR12 | (segmentMap[2] << offset);
        LCDDR17 = LCDDR17 | (segmentMap[3] << offset);
    }

}

void mapLCDSegments(uint8_t *segmentMap, char ch) {

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
            break;
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


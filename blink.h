#include "include/avr/io.h"

void initClk(void);
uint16_t blink(void);

#define CLKPERIOD 15625 // (8 M / 256) / 2

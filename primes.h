#include "include/avr/io.h"

uint16_t writeChar(char ch, int pos);
uint16_t mapLCDSegments(uint8_t *segmentMap, char ch);
uint16_t writeLong(long i);
uint16_t primes(void);
uint16_t isPrime(long i);


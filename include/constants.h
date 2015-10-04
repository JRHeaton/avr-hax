#ifndef HAX_CONSTANTS_H
#define HAX_CONSTANTS_H

#include <stdint.h>

#define LOW 0
#define HIGH 1

// Bitwise macros
#define SET(b, bit) ((b) |= (1 << bit))
#define UNSET(b, bit) ((b) &= ~(1 << bit))
#define TOGGLE(b, bit) ((b) ^= (1 << bit))

#endif

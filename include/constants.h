#ifndef HAX_CONSTANTS_H
#define HAX_CONSTANTS_H

typedef unsigned char byte;
typedef volatile byte addr;

// Bitwise macros
#define BYTE(bit) (1 << (bit))
#define SET(b, bit) ((b) |= BYTE(bit))
#define UNSET(b, bit) ((b) &= ~BYTE(bit))

#endif

#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <avr/io.h>

struct Port {
public:

    volatile uint8_t *direction;
    volatile uint8_t *input;
    volatile uint8_t *output;
    
};

#define DEFINE_PORT(xx) Port Port ## xx = { &DDR ## xx, &PIN ## xx, &PORT ##xx };

#ifdef PORTB
DEFINE_PORT(B);
#endif

#ifdef PORTC
DEFINE_PORT(C);
#endif

#ifdef PORTD
DEFINE_PORT(D);
#endif

#endif
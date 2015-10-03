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

#ifdef PORTB
extern Port PortB;
#endif

#ifdef PORTC
extern Port PortC;
#endif

#ifdef PORTD
extern Port PortD;
#endif

#endif
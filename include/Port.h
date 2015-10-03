#ifndef PORT_H
#define PORT_H

#include <stdint.h>

struct Port {
public:

    volatile uint8_t *direction;
    volatile uint8_t *input;
    volatile uint8_t *output;
};

#endif
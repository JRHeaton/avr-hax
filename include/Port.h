#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

#include "Pin.h"

struct Port {
public:

    volatile uint8_t *direction;
    volatile uint8_t *input;
    volatile uint8_t *output;
    
    Port(volatile uint8_t *direction,
         volatile uint8_t *input,
         volatile uint8_t *output)
    : direction(direction), input(input), output(output) { }
    
    void setPinMode(uint8_t pin, uint8_t mode);
    void setAllPinMode(uint8_t mode);
    void writeByte(uint8_t byte);
    void writePin(uint8_t pin, bool high);
    uint8_t readByte();
    bool readPin(uint8_t pin);
    bool togglePin(uint8_t pin);
    Pin operator[] (uint8_t index);
};

#define DEFINE_PORT(xx) Port xx = Port(&DDR ## xx, &PIN ## xx, &PORT ##xx);

#ifndef NO_PORTMAP

struct {
#ifdef PORTA
    DEFINE_PORT(A)
#endif
    
#ifdef PORTB
    DEFINE_PORT(B)
#endif
    
#ifdef PORTC
    DEFINE_PORT(C)
#endif
    
#ifdef PORTD
    DEFINE_PORT(D)
#endif
    
#ifdef PORTE
    DEFINE_PORT(E)
#endif
    
#ifdef PORTF
    DEFINE_PORT(F)
#endif
} ports;

#endif

#endif
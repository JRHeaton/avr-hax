#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

#include "Pin.h"

struct PortType {
public:
    virtual void setPinMode(uint8_t pin, uint8_t mode) = 0;
    virtual void setHighNibbleMode(uint8_t mode) = 0;
    virtual void setLowNibbleMode(uint8_t mode) = 0;
    virtual void setAllPinMode(uint8_t mode) = 0;
    virtual void writeByte(uint8_t byte) = 0;
    virtual void writeHighNibble(uint8_t byte) = 0;
    virtual void writeLowNibble(uint8_t byte) = 0;
    virtual void writePin(uint8_t pin, bool high) = 0;
    virtual uint8_t readByte() = 0;
    virtual bool readPin(uint8_t pin) = 0;
    virtual bool togglePin(uint8_t pin) = 0;
};

struct Port : PortType {
public:

    volatile uint8_t *direction;
    volatile uint8_t *input;
    volatile uint8_t *output;
    
    Port(volatile uint8_t *direction,
         volatile uint8_t *input,
         volatile uint8_t *output)
    : direction(direction), input(input), output(output) { }
    
    virtual void setPinMode(uint8_t pin, uint8_t mode);
    virtual void setAllPinMode(uint8_t mode);
    virtual void setHighNibbleMode(uint8_t mode);
    virtual void setLowNibbleMode(uint8_t mode);
    virtual void writeByte(uint8_t byte);
    virtual void writeHighNibble(uint8_t byte);
    virtual void writeLowNibble(uint8_t byte);
    virtual void writePin(uint8_t pin, bool high);
    virtual uint8_t readByte();
    virtual bool readPin(uint8_t pin);
    virtual bool togglePin(uint8_t pin);
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
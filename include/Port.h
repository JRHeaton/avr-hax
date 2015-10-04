#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

#include "Pin.h"

struct PortType {
public:
    virtual void setPinMode(uint8_t pin, uint8_t mode) = 0;
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
    
    virtual void setPinMode(uint8_t pin, uint8_t mode) {
        switch (mode) {
            case INPUT: UNSET(*direction, pin); break;
            case OUTPUT: SET(*direction, pin); break;
            case INPUT_PULLUP:
                UNSET(*direction, pin);
                SET(*output, pin);
                break;
        }
    }
    
    virtual void setAllPinMode(uint8_t mode) {
        switch (mode) {
            case INPUT: *direction = 0;  break;
            case OUTPUT: *direction = 0xFF; break;
            case INPUT_PULLUP:
                *direction = 0;
                *output = 0xFF;
                break;
        }
    }
    
    virtual void writeByte(uint8_t byte) {
        *output = byte;
    }
    
    virtual void writeHighNibble(uint8_t byte) {
        uint8_t value = *output;
        value &= 0x0F;
        value |= byte & 0xF0;
        *output = value;
    }
    
    virtual void writeLowNibble(uint8_t byte) {
        uint8_t value = *output;
        value &= 0xF0;
        value |= byte & 0x0F;
        *output = value;
    }
    
    virtual void writePin(uint8_t pin, bool high) {
        if (high)   { SET(*output, pin); }
        else        { UNSET(*output, pin); }
    }
    
    virtual uint8_t readByte() {
        return *input;
    }
    
    virtual bool readPin(uint8_t pin) {
        return (*input & _BV(pin));
    }
    
    virtual bool togglePin(uint8_t pin) {
        *output ^= uint8_t(pin);
        return *output;
    }
    
    Pin operator[] (uint8_t index) {
        return { this, index };
    }
};

#define DEFINE_PORT(xx) Port xx = Port(&DDR ## xx, &PIN ## xx, &PORT ##xx);

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
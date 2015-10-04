#ifndef PinMap_h
#define PinMap_h

#include "Pin.h"
#include "Port.h"

#ifdef __AVR_AT90USB162__
Pin pins[] = {
    { NULL, 0 },        // offset by 1 (reference pins by board value)
    { NULL, 0 },        // XTAL1
    { &ports.C, 0 },    // XTAL2
    { NULL, 0 },        // GND
    { NULL, 0 },        // VCC
    { &ports.C, 2 },    // PC2
    { &ports.D, 0 },    // PD0
    { &ports.D, 1 },    // PD1
    { &ports.D, 2 },    // PD2
    { &ports.D, 3 },    // PD3
    { &ports.D, 4 },    // PD4
    { &ports.D, 5 },    // PD5
    { &ports.D, 6 },    // PD6
    { &ports.D, 7 },    // PD7
    { &ports.B, 0 },    // PB0
    { &ports.B, 1 },    // PB1
    { &ports.B, 2 },    // PB2
    { &ports.B, 3 },    // PB3
    { &ports.B, 4 },    // PB4
    { &ports.B, 5 },    // PB5
    { &ports.B, 6 },    // PB6
    { &ports.B, 7 },    // PB7
    { &ports.C, 7 },    // PC7
    { &ports.C, 6 },    // PC6
    { &ports.C, 1 },    // PC1/RESET
    { &ports.C, 5 },    // PC5
    { &ports.C, 4 },    // PC4
};
#endif

#endif /* PinMap_h */

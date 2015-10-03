#ifndef PinMap_h
#define PinMap_h

#include "Pin.h"

#ifdef __AVR_AT90USB162__
Pin pins[] = {
    { NULL, 0},     // offset by 1 (reference pins by board value)
    { NULL, 0 },    // XTAL1
    { &PortC, 0},   // XTAL2
    { NULL, 0 },    // GND
    { NULL, 0 },    // VCC
    { &PortC, 2},   // PC2
    { &PortD, 0},   // PD0
    { &PortD, 1},   // PD1
    { &PortD, 2},   // PD2
    { &PortD, 3},   // PD3
    { &PortD, 4},   // PD4
    { &PortD, 5},   // PD5
    { &PortD, 6},   // PD6
    { &PortD, 7},   // PD7
    { &PortB, 0},   // PB0
    { &PortB, 1},   // PB1
    { &PortB, 2},   // PB2
    { &PortB, 3},   // PB3
    { &PortB, 4},   // PB4
    { &PortB, 5},   // PB5
    { &PortB, 6},   // PB6
    { &PortB, 7},   // PB7
    { &PortC, 7},   // PC7
    { &PortC, 6},   // PC6
    { &PortC, 1},   // PC1/RESET
    { &PortC, 5},   // PC5
    { &PortC, 4},   // PC4
};
#endif

#endif /* PinMap_h */

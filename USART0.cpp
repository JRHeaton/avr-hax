//
//  USART0.cpp
//  hax
//
//  Created by John Heaton on 9/20/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#include "USART0.h"

#include <avr/io.h>
#include <util/delay.h>

void USART0::init(unsigned long baud) {
    unsigned long adjusted = (F_CPU/(16*baud))-1;
    UBRR0H = (adjusted >> 8) & 0xFF;
    UBRR0L = adjusted & 0xFF;
    
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART0::enableInput(bool enable) {
    if (enable) { SET(UCSR0B, RXEN0); }
    else        { UNSET(UCSR0B, RXEN0); }
}

void USART0::enableOutput(bool enable) {
    if (enable) { SET(UCSR0B, TXEN0); }
    else        { UNSET(UCSR0B, TXEN0); }
}

void USART0::write(byte data) {
    while (!(UCSR0A & BYTE(UDRE0))) {}
    UDR0 = data;
}

byte USART0::read() {
    while (!(UCSR0A & BYTE(RXC0))) {}
    return UDR0;
}

int USART0::file_put(char c, FILE *f) {
    USART0::write(c);
    return 0;
}

int USART0::file_get(FILE *f) {
    return USART0::read();
}
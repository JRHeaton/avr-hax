//
//  USART.cpp
//  hax
//
//  Created by John Heaton on 9/20/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#include "USART.h"

#include <avr/io.h>
#include <util/delay.h>

#ifdef UDR0
extern USART USART1((USART::Registers){ &UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0 });
#endif

#ifdef UDR1
USART USART1((USART::Registers){ &UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1 });
#endif

USART::USART(Registers regs) : regs(regs) {    
    file.put = USART::file_put;
    file.get = USART::file_get;
    file.udata = this;
    file.flags = __SRD | __SWR;
}

void USART::init(unsigned long baud) {
    unsigned long adjusted = (F_CPU/(16*baud))-1;
    *regs.UBRRH = (adjusted >> 8) & 0xFF;
    *regs.UBRRL = adjusted & 0xFF;

    *regs.UCSRC = (1 << 2) | (1 << 1); // 8-bit
}

void USART::enableInput(bool enable) {
    if (enable) { SET(*regs.UCSRB, 4); }
    else        { UNSET(*regs.UCSRB, 4); }
}

void USART::enableOutput(bool enable) {
    if (enable) { SET(*regs.UCSRB, 3); }
    else        { UNSET(*regs.UCSRB, 3); }
}

void USART::write(uint8_t data) {
    while ((*regs.UCSRA & (1 << 5)) == 0) {}
    *regs.UDR = data;
}

uint8_t USART::read() {
    while (!(*regs.UCSRA & (1 << 7))) {}
    return *regs.UDR;
}

int USART::file_put(char c, FILE *f) {
    if (f != NULL) {
        USART *val = (USART *)f->udata;
        if (val != NULL) {
            val->write(c);
        }
    }
    return 0;
}

int USART::file_get(FILE *f) {
    if (f != NULL) {
        USART *val = (USART *)f->udata;
        if (val != NULL) {
            return val->read();
        }
    }
    return 0;
}
//
//  USART.h
//  hax
//
//  Created by John Heaton on 9/20/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#ifndef USART_h
#define USART_h

#include "constants.h"
#include <stdio.h>
#include <avr/io.h>

struct USART {
public:
    
    struct Registers {
        volatile uint8_t *UBRRH; // baud rate high
        volatile uint8_t *UBRRL; // baud rate low
        volatile uint8_t *UCSRA; // control/status A
        volatile uint8_t *UCSRB; // control/status B
        volatile uint8_t *UCSRC; // control/status C
        volatile uint8_t *UDR;   // i/o data
    };
    
    USART(Registers regs);
    
    void init(unsigned long baud);
    void enableInput(bool enable);
    void enableOutput(bool enable);
    void write(uint8_t data);
    uint8_t read();
    
    FILE file;
    
protected:
    
    Registers regs;
    
    static int file_put(char c, FILE *f);
    static int file_get(FILE *f);
};

#ifdef UDR0
extern USART USART1;
#endif

#ifdef UDR1
extern USART USART1;
#endif

#endif /* USART_h */

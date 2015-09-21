//
//  USART0.h
//  hax
//
//  Created by John Heaton on 9/20/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#ifndef USART0_h
#define USART0_h

#include "constants.h"
#include <stdio.h>

class USART0 {
public:
    
    static void init(unsigned long baud);
    static void enableInput(bool enable);
    static void enableOutput(bool enable);
    static void write(byte data);
    static byte read();
    static int file_put(char c, FILE *f);
    static int file_get(FILE *f);
};

#endif /* USART0_h */

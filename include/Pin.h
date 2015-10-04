//
//  Pin.h
//  hax
//
//  Created by John Heaton on 10/3/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#ifndef Pin_h
#define Pin_h

#include <stdint.h>
#include "constants.h"

struct Port;

// Pin modes
#define INPUT           0
#define INPUT_PULLUP    1
#define OUTPUT          2

struct Pin {
public:
    
    Port *port;
    uint8_t pin_number;
        
    void setMode(uint8_t mode);
    void write(uint8_t value);
    void toggle();
};

#endif /* Pin_h */

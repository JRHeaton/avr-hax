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
#include "Port.h"

#define INPUT           0
#define INPUT_PULLUP    1
#define OUTPUT          2

struct Pin {
public:
    Port *port;
    uint8_t pin_number;
    
    void setMode(uint8_t mode) {
        switch (mode) {
            case INPUT:
                UNSET(*port->direction, pin_number);
                break;
            case OUTPUT:
                SET(*port->direction, pin_number);
                break;
            case INPUT_PULLUP:
                UNSET(*port->direction, pin_number);
                high();
                break;
        }
    }
    
    void high() { SET(*port->output, pin_number); }
    void low()  { UNSET(*port->output, pin_number); }
    void toggle() { TOGGLE(*port->output, pin_number); }
    
    void operator= (uint8_t value) {
        if (!value) { low(); } else { high(); }
    }
};

#endif /* Pin_h */

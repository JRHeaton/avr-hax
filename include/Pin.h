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

#define INPUT           0
#define INPUT_PULLUP    1
#define OUTPUT          2

struct Pin {
public:
    
    volatile uint8_t *port_direction, *port_output, *port_input;
    uint8_t pin_number;
    
    void setMode(uint8_t mode) {
        switch (mode) {
            case INPUT:
                UNSET(*port_direction, pin_number);
                break;
            case OUTPUT:
                SET(*port_direction, pin_number);
                break;
            case INPUT_PULLUP:
                UNSET(*port_direction, pin_number);
                high();
                break;
        }
    }
    
    void high() { SET(*port_output, pin_number); }
    void low()  { UNSET(*port_output, pin_number); }
    void toggle() { TOGGLE(*port_output, pin_number); }
};

#endif /* Pin_h */

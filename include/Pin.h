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
        
    void setMode(uint8_t mode);
    void high();
    void low();
    void toggle();
    
    void operator= (uint8_t value);
};

#endif /* Pin_h */

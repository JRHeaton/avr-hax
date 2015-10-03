//
//  Pin.cpp
//  hax
//
//  Created by John Heaton on 10/3/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#include "Pin.h"

void Pin::setMode(uint8_t mode) {
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

void Pin::high() {
    SET(*port->output, pin_number);
}

void Pin::low()  {
    UNSET(*port->output, pin_number);
}

void Pin::toggle() {
    TOGGLE(*port->output, pin_number);
}

void Pin::operator= (uint8_t value) {
    if (!value) { low(); } else { high(); }
}

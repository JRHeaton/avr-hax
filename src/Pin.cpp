//
//  Pin.cpp
//  hax
//
//  Created by John Heaton on 10/3/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#include "Port.h"
#include "Pin.h"

void Pin::setMode(uint8_t mode) {
    port->setPinMode(pin_number, mode);
}

void Pin::write(uint8_t value) {
    port->writePin(pin_number, value != 0);
}

void Pin::toggle() {
    port->togglePin(pin_number);
}

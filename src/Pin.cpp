//
//  Pin.cpp
//  hax
//
//  Created by John Heaton on 10/3/15.
//  Copyright © 2015 John Heaton. All rights reserved.
//

#include "Port.h"
#include "Pin.h"

#include <util/delay.h>

void Pin::setMode(uint8_t mode) {
    port->setPinMode(pin_number, mode);
}

void Pin::write(bool value) {
    port->writePin(pin_number, value);
}

bool Pin::read() {
    return port->readPin(pin_number);
}

void Pin::toggle() {
    port->togglePin(pin_number);
}

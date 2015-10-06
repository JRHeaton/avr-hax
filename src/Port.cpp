#include "Port.h"

void Port::setPinMode(uint8_t pin, uint8_t mode) {
    switch (mode) {
        case INPUT:
            UNSET(*direction, pin);
            UNSET(*output, pin); // disable pull-up
            break;
        case OUTPUT: SET(*direction, pin); break;
        case INPUT_PULLUP:
            UNSET(*direction, pin);
            SET(*output, pin);
            break;
    }
}

void Port::setAllPinMode(uint8_t mode) {
    switch (mode) {
        case INPUT:
            *output = 0;
            *direction = 0;
            break;
        case OUTPUT: *direction = 0xFF; break;
        case INPUT_PULLUP:
            *direction = 0;
            *output = 0xFF;
            break;
    }
}

void Port::writeByte(uint8_t byte) {
    *output = byte;
}

void Port::writePin(uint8_t pin, bool high) {
    if (high)   { SET(*output, pin); }
    else        { UNSET(*output, pin); }
}

uint8_t Port::readByte() {
    return *input;
}

bool Port::readPin(uint8_t pin) {
    return (*input & _BV(pin));
}

bool Port::togglePin(uint8_t pin) {
    *output ^= uint8_t(pin);
    return *output;
}

Pin Port::operator[] (uint8_t index) {
    return { this, index };
}

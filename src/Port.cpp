#include "Port.h"

void Port::setPinMode(uint8_t pin, uint8_t mode) {
    switch (mode) {
        case INPUT: UNSET(*direction, pin); break;
        case OUTPUT: SET(*direction, pin); break;
        case INPUT_PULLUP:
            UNSET(*direction, pin);
            SET(*output, pin);
            break;
    }
}

void Port::setAllPinMode(uint8_t mode) {
    switch (mode) {
        case INPUT: *direction = 0;  break;
        case OUTPUT: *direction = 0xFF; break;
        case INPUT_PULLUP:
            *direction = 0;
            *output = 0xFF;
            break;
    }
}

void Port::setHighNibbleMode(uint8_t mode) {
    uint8_t value = *direction;
    value &= 0x0F;
    
    switch (mode) {
        case INPUT:
            *direction = value;
            break;
        case OUTPUT:
            value |= 0xF0;
            *direction = value;
            break;
        case INPUT_PULLUP:
            *direction = value;
            *output |= 0xF0;
            break;
    }
}

void Port::setLowNibbleMode(uint8_t mode) {
    uint8_t value = *direction;
    value &= 0xF0;
    
    switch (mode) {
        case INPUT:
            *direction = value;
            break;
        case OUTPUT:
            value |= 0x0F;
            *direction = value;
            break;
        case INPUT_PULLUP:
            *direction = value;
            *output |= 0x0F;
            break;
    }
}

void Port::writeByte(uint8_t byte) {
    *output = byte;
}

void Port::writeHighNibble(uint8_t byte) {
    uint8_t value = *output;
    value &= 0x0F;
    value |= byte & 0xF0;
    *output = value;
}

void Port::writeLowNibble(uint8_t byte) {
    uint8_t value = *output;
    value &= 0xF0;
    value |= byte & 0x0F;
    *output = value;
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

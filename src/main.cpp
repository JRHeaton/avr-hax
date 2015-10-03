#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "Pin.h"
#include "PinMap.h"

__attribute__((section(".eeprom")))
unsigned char sequence = 0b01010010;

int main() {
    pins[20].setMode(OUTPUT);
    
    while (1) {
        pins[20].toggle();
        _delay_ms(100);
    }
    
    return 0;
}

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "Pin.h"

__attribute__((section(".eeprom")))
unsigned char greeting[] = "what's up?";

int main() {
    
    Port portB = {&DDRB, &PINB, &PORTB};
    Pin pb6 = {&portB, PB6};
    pb6.setMode(OUTPUT);
    
    while (1) {
        _delay_ms(1000);
        pb6.toggle();
    }
    
    return 0;
}

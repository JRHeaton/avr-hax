#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "Pin.h"

__attribute__((section(".eeprom")))
unsigned char greeting[] = "what's up?";

int main() {
    
    Pin pb6 = { &DDRB, &PORTB, &PINB, PB6 };
    pb6.setMode(OUTPUT);
    
    while (1) {
        pb6.toggle();
        _delay_ms(500);
    }
    
    return 0;
}

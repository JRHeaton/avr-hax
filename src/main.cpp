#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "Pin.h"

__attribute__((section(".eeprom")))
unsigned char sequence = 0b01010010;

int main() {
    
    Port portB = {&DDRB, &PINB, &PORTB};
    Pin pb6 = {&portB, PB6};
    pb6.setMode(OUTPUT);
    
    unsigned char _sequence = eeprom_read_byte(0);
    
    while (1) {
        for (uint8_t i=0;i<8;++i) {
            pb6 = (_sequence & (1 << i));
            _delay_ms(1000);
        }
    }
    
    return 0;
}

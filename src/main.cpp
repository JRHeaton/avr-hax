#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

__attribute__((section(".eeprom")))
unsigned char greeting[] = "what's up?";

int main() {
    return 0;
}

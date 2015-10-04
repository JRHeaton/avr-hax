#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>

#include "Pin.h"
#include "PinMap.h"
#include "LCD.h"
#include "USART0.h"

__attribute__((section(".eeprom")))
unsigned char sequence = 0x0;

// Necessary since the actual standard lib isn't here to implement it
extern "C" void __cxa_pure_virtual() {}

int main() {
    USART1.init(9600);
    USART1.enableInput(true);
    USART1.enableOutput(true);
    
    LCD m(&pins[11], // RS
          &pins[12], // RW
          &pins[13], // E
          &ports.B);
    
    m.functionSet(false, true, false);
    m.displayMode(true, true, true);
    m.clearDisplay();
    m.returnHome();
    
    stdin = &USART1.file;
    stdout = &USART1.file;
    
    struct {
        Pin *power;
        Pin *menu;
        Pin *down;
        Pin *up;
    } vape = { &pins[22], &pins[23], &pins[6], &pins[7] };
    vape.power->setMode(OUTPUT);
    vape.menu->setMode(OUTPUT);
    vape.up->setMode(OUTPUT);
    vape.down->setMode(OUTPUT);
    
    bool blink = true;
    
#define STROBE(member, duration)    \
vape.member->write(1);          \
_delay_ms(duration);            \
vape.member->write(0);          \
_delay_ms(26);

    
    while (1) {
        printf("\n\r>> ");

        char cmd[0x20] = { 0 };
        uint8_t i=0;
        while (1) {
            char c = getchar();
            if (c == 0xD) {
                break;
            } else {
                cmd[i++] = c;
                putchar(c);
            }
        }
        
        if (!strcmp(cmd, "clear")) {
            m.clearDisplay();
        }
        else if (!strcmp(cmd, "blink")) {
            m.displayMode(true, true, (blink = !blink));
        }
        if (!strcmp(cmd, " ")) {
            STROBE(power, 50);
        }
        else if (!strcmp(cmd, "k")) {
            STROBE(up, 50);
        }
        else if (!strcmp(cmd, "j")) {
            STROBE(down, 50);
        }
        else if (!strcmp(cmd, "m")) {
            STROBE(menu, 50);
        }
        else if (!strcmp(cmd, "f1")) {
            STROBE(menu, 50);
            STROBE(menu, 50);
            STROBE(menu, 50);
            STROBE(menu, 50);
            STROBE(up, 50);
        }
        else {
            m.sendString(cmd);
        }
    }
    
    return 0;
}

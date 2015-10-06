#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>

#include "Pin.h"
#include "PinMap.h"
#include "LCD.h"
#include "USART.h"

__attribute__((section(".eeprom")))
unsigned char sequence = 0x0;

int main() {
    USART serial = USART1;
    
    serial.init(9600);
    serial.enableInput(true);
    serial.enableOutput(true);
    stdin = &serial.file;
    stdout = &serial.file;
    
    LCD m(true,
          false,
          &pins[11], // RS
          &pins[12], // RW
          &pins[13], // E
          &pins[21],
          &pins[20],
          &pins[19],
          &pins[18]);
    
    m.init();
    printf("--LCD initialized--\n\r");
//    m.entryModeSet(true, false);
    m.displayMode(true, true, true);
    printf("-display mode set\n\r");
    m.clearDisplay();
    printf("-display cleared\n\r");
    m.returnHome();
    printf("-display home\n\r");
    
    
    m.sendCharacter('a');
    m.sendCharacter('j');
    
//    struct {
//        Pin *power;
//        Pin *menu;
//        Pin *down;
//        Pin *up;
//    } vape = { &pins[22], &pins[23], &pins[6], &pins[7] };
//    vape.power->setMode(OUTPUT);
//    vape.menu->setMode(OUTPUT);
//    vape.up->setMode(OUTPUT);
//    vape.down->setMode(OUTPUT);
    
    bool blink = true;

#define STROBE(member, duration)    \
vape.member->write(1);          \
_delay_ms(duration);            \
vape.member->write(0);          \
_delay_ms(26);
    
    while (1) {
        printf("\n\r>> ");

        char cmd[80] = { 0 };
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
        
//        printf("command received\n");
        
        if (!strcmp(cmd, "clear")) {
            m.clearDisplay();
        }
        else if (!strcmp(cmd, "blink")) {
            m.displayMode(true, true, (blink = !blink));
        }
//        else if (!strcmp(cmd, " ")) {
//            STROBE(power, 50);
//        }
//        else if (!strcmp(cmd, "k")) {
//            STROBE(up, 50);
//        }
//        else if (!strcmp(cmd, "j")) {
//            STROBE(down, 50);
//        }
//        else if (!strcmp(cmd, "m")) {
//            STROBE(menu, 50);
//        }
//        else if (!strcmp(cmd, "f1")) {
//            STROBE(menu, 50);
//            STROBE(menu, 50);
//            STROBE(menu, 50);
//            STROBE(menu, 50);
//            STROBE(up, 50);
//        }
        else {
            m.sendString(cmd);
        }
    }
    
    return 0;
}

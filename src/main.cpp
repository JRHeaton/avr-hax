#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

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
    
    m.sendString("yo");
    
    while (1) {
        uint8_t byte = USART1.read();
        m.sendCharacter(byte);
        USART1.write(byte);
    }
    
    return 0;
}

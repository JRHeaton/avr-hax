#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "LCD.h"
#include "USART0.h"

// 8 bit LCD logic pins
#define LCD_DATA_DIRECTION      DDRB
#define LCD_DATA_PORT           PORTB
#define LCD_DATA_INPUT          PINB

// LCD control pins
#define LCD_CONTROL_DIRECTION   DDRC
#define LCD_CONTROL_PORT        PORTC
#define LCD_RS                  PC5
#define LCD_RW                  PC4
#define LCD_E                   PC3

// Test LED
#define LED_DIRECTION           DDRC
#define LED_PORT                PORTC
#define LED                     PC2

// Used for stdout
FILE lcd_file;
FILE serial_file;

int main() {
    LED_DIRECTION = 0xFF;
    SET(LED_PORT, LED);
    
    USART0::init(9600);
    USART0::enableInput(true);
    USART0::enableOutput(true);
    
    LCD m(&LCD_DATA_DIRECTION, &LCD_DATA_PORT, &LCD_DATA_INPUT,
          &LCD_CONTROL_DIRECTION, &LCD_CONTROL_PORT,
          LCD_RS, LCD_RW, LCD_E);
        
    lcd_file.put = LCD::file_put;
    lcd_file.udata = &m;
    lcd_file.flags = __SWR;
    stdout = &lcd_file;

    m.functionSet(
        false,   /* 8 bit logic */
        true,   /* 2-line display */
        false   /* 5x8 font */
    );
    m.clearDisplay();
    m.returnHome();
    m.displayMode(
        true,   /* display on */
        true,   /* cursor on */
        true    /* cursor blink */
    );
    
    serial_file.put = USART0::file_put;
    serial_file.get = USART0::file_get;
    serial_file.udata = 0;
    serial_file.flags = __SRD | __SWR;
    stdin = &serial_file;

    while(1) {
        byte b = getchar();
        fputc(b, &serial_file);
        
        if (b == '\\') {
            m.clearDisplay();
            m.returnHome();
            fputs("\n\r", &serial_file);
            
            continue;
        }
        else if (b == 0x1B) { //esc
            byte x = getchar();
            
            if (x == '[') {
                byte y = getchar();
                
                if (y == 'A') {
                    m.setDRAMAddress(0x0);
                    continue;
                }
                else if (y == 'B') {
                    m.setDRAMAddress(0x40);
                    continue;
                }
            }
        }
        
        m.sendCharacter(b);
    }
    
    return 0;
}

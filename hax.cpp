#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "LCD.h"

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

int main() {
    LED_DIRECTION = 0xFF;
    SET(LED_PORT, LED);
    
    LCD m(&LCD_DATA_DIRECTION, &LCD_DATA_PORT, &LCD_DATA_INPUT,
          &LCD_CONTROL_DIRECTION, &LCD_CONTROL_PORT,
          LCD_RS, LCD_RW, LCD_E);
    
    lcd_file.put = LCD::file_put;
    lcd_file.udata = &m;
    lcd_file.flags = __SWR;
    stdout = &lcd_file;

    m.functionSet(
        true,   /* 8 bit logic */
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
    
    puts("1. Get an LCD");
    m.setDRAMAddress(0x40);
    puts("2. Use this lib");
    
    while(1) {}
    
    return 0;
}

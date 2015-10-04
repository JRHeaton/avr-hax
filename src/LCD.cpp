#include "LCD.h"
#include <util/delay.h>

LCD::LCD(Pin *RS,
         Pin *RW,
         Pin *E,
         PortType *DB) : RS(RS), RW(RW), E(E), DB(DB)
{
    RS->setMode(OUTPUT);
    RW->setMode(OUTPUT);
    E->setMode(OUTPUT);
    
    file.put = LCD::file_put;
    file.udata = this;
    file.flags = __SRD | __SWR;
}

void LCD::selectDR() {
    RS->write(HIGH);
}

void LCD::selectIR() {
    RS->write(LOW);
}

void LCD::setRead(bool read) {
    RW->write(read);
}

void LCD::setEnable(bool enable) {
    E->write(enable);
}

void LCD::strobeEnable() {
    E->write(HIGH);
    _delay_ms(LCD_STROBE_DURATION);
    E->write(LOW);
}

uint8_t LCD::readBusyFlagAndAC() {
    setRead(true);
    selectIR();
    
    if (_8bitmode) {
        DB->setAllPinMode(INPUT);
        
        E->write(HIGH);
        _delay_ms(LCD_STROBE_DURATION);
        uint8_t ret = DB->readByte();
        E->write(LOW);
        
        DB->setAllPinMode(OUTPUT);
        
        return ret;
    } else {
        
        // Set high 4 pins to input
        DB->setHighNibbleMode(INPUT);
        
        // Turn on E, wait
        E->write(HIGH);
        _delay_ms(LCD_STROBE_DURATION);

        // Read high 4 bits, turn off E
        uint8_t high = DB->readByte() & 0xF0;
        E->write(LOW);
        
        // Turn on E, wait
        E->write(HIGH);
        _delay_ms(LCD_STROBE_DURATION);

        // Read low 4 bits, turn off E
        uint8_t low = DB->readByte() & 0x0F;
        E->write(LOW);

        // Set high 4 pins to output
        DB->setHighNibbleMode(OUTPUT);
        
        return (high | low);
    }
    
    return 0;
}

void LCD::busyWait() {
    while ((readBusyFlagAndAC() & 0x80) == 0x80) {}
}

void LCD::sendCommand(uint8_t cmd) {
    busyWait();
    
    setRead(false);
    selectIR();

    if (_8bitmode) {
        DB->writeByte(cmd);
        strobeEnable();
        DB->writeByte(0);
    } else {
        uint8_t high = cmd & 0xF0;
        uint8_t low = (cmd & 0x0F) << 4;
        
        // Send high 4 bits
        DB->writeHighNibble(high);
        strobeEnable();
        
        // Send low 4 bits
        DB->writeHighNibble(low);
        strobeEnable();
        
        // Reset bus lines LOW
        DB->writeHighNibble(0);
    }
}

void LCD::sendCharacter(char character) {
    busyWait();
    
    setRead(false);
    selectDR();

    if (_8bitmode) {
        DB->writeByte(character);
        strobeEnable();
        DB->writeByte(0);
    } else {
        uint8_t high = character & 0xF0;
        uint8_t low = (character & 0x0F) << 4;
        
        // Send high 4 bits
        DB->writeHighNibble(high);
        strobeEnable();
        
        // Send low 4 bits
        DB->writeHighNibble(low);
        strobeEnable();
        
        // Reset bus lines LOW
        DB->writeHighNibble(0);
    }
}

void LCD::sendString(const char *string) {
    char *str = (char *)string;
    while (*str != '\0') {
        sendCharacter(*str++);
    }
}

void LCD::clearDisplay() {
    sendCommand(0x01);
}

void LCD::returnHome() {
    sendCommand(0x02);
}

void LCD::displayMode(bool displayOn, bool cursorOn, bool cursorBlink) {
    sendCommand((1 << 3) | (displayOn << 2) | (cursorOn << 1) | cursorBlink);
}

void LCD::entryModeSet(bool increment, bool displayShift) {
    sendCommand((1 << 2) | (increment << 1) | displayShift);
}

void LCD::shift(bool followDisplayShift,
                bool directionRight) {
    sendCommand((1 << 4) | (followDisplayShift << 3) | (directionRight << 2));
}

void LCD::functionSet(bool _8bit, bool _2line, bool useBigFont) {
    uint8_t cmd = (1 << 5) | (_8bit << 4) | (_2line << 3) | (useBigFont << 2);
    
    // set up DB lines as output initially
    if (_8bit) {
        DB->setAllPinMode(OUTPUT);
    } else {
        DB->setHighNibbleMode(OUTPUT);
        
        // Set up for special command
        busyWait();
        setRead(false);
        selectIR();
        
        // Send high nibble of "set 4bit" command
        DB->writeHighNibble(cmd);
        strobeEnable();
    }
    
    _8bitmode = _8bit;
    
    // Send (initial) command
    sendCommand(cmd);
}

void LCD::setDRAMAddress(uint8_t address) {
    sendCommand((1 << 7) | (address & 0x7F));
}

int LCD::file_put(char c, FILE *f) {
    if (!f) {
        return 1;
    }
    if (c == '\n') {
        return 0;
    }
    
    LCD *lcd = (LCD *)f->udata;
    if (lcd != NULL) {
        lcd->sendCharacter(c);
    }
    
    return 0;
}
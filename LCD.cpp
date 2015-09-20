#include "LCD.h"
#include <util/delay.h>

#define LCD_STROBE_DURATION     1

LCD::LCD(addr *data_dir, addr *data_port, addr *data_pin,
    addr *ctrl_dir, addr *ctrl_port,
    byte rs, byte rw, byte e) :
data_dir(data_dir), data_port(data_port), data_pin(data_pin),
ctrl_dir(ctrl_dir), ctrl_port(ctrl_port),
rs(rs), rw(rw), e(e)
{
    *ctrl_dir = BYTE(rs) | BYTE(rw) | BYTE(e);
}

void LCD::selectDR() {
    SET(*ctrl_port, rs);
}

void LCD::selectIR() {
    UNSET(*ctrl_port, rs);
}

void LCD::setRead(bool read) {
    if (read)   { SET(*ctrl_port, rw); }
    else        { UNSET(*ctrl_port, rw); }
}

void LCD::setEnable(bool enable) {
    if (enable) { SET(*ctrl_port, e); }
    else        { UNSET(*ctrl_port, e); }
}

void LCD::strobeEnable() {
    setEnable(true);
    _delay_ms(LCD_STROBE_DURATION);
    setEnable(false);
}

unsigned char LCD::readBusyFlag() {
    // direction = input
    *data_dir = 0;
    
    setRead(true);
    selectIR();
    
    setEnable(true);
    _delay_ms(LCD_STROBE_DURATION);
    unsigned char ret = *data_pin;
    setEnable(false);
    
    // direction = output
    *data_dir = 0xFF;
    
    return ret;
}

void LCD::busyWait() {
    while ((readBusyFlag() & 0x80) == 0x80) {}
}

void LCD::sendCommand(unsigned char cmd) {
    busyWait();
    
    *data_port = cmd;
    setRead(false);
    selectIR();
    
    strobeEnable();
    
    *data_port = 0;
}

void LCD::sendCharacter(char character) {
    busyWait();
    
    *data_port = character;
    
    setRead(false);
    selectDR();
    strobeEnable();
    
    *data_port = 0;
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

void LCD::functionSet(bool _8bit, bool _2line, bool useBigFont) {
    sendCommand((1 << 5) | (_8bit << 4) | (_2line << 3) | (useBigFont << 2));
}

void LCD::setDRAMAddress(byte address) {
    sendCommand((1 << 7) | (address & 0x7F));
}

int LCD::file_put(char c, FILE *f) {
    if (c == '\n') {
        return 0;
    }
    
    LCD *lcd = (LCD *)f->udata;
    if (lcd != NULL) {
        lcd->sendCharacter(c);
    }
    
    return 0;
}
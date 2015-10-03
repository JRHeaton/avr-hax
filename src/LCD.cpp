#include "LCD.h"
#include <util/delay.h>

#include "USART0.h"

LCD::LCD(addr *data_dir, addr *data_port, addr *data_pin,
    addr *ctrl_dir, addr *ctrl_port,
    byte rs, byte rw, byte e) :
data_dir(data_dir), data_port(data_port), data_pin(data_pin),
ctrl_dir(ctrl_dir), ctrl_port(ctrl_port),
rs(rs), rw(rw), e(e)
{
    *ctrl_dir = BYTE(rs) | BYTE(rw) | BYTE(e);
    *data_dir = 0XFF;
    
    file.put = LCD::file_put;
    file.udata = this;
    file.flags = __SRD | __SWR;
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

byte LCD::readBusyFlagAndAC() {
    setRead(true);
    selectIR();
    
    if (_8bitmode) {
        *data_dir = 0;
        
        setEnable(true);
        _delay_ms(LCD_STROBE_DURATION);
        unsigned char ret = *data_pin;
        setEnable(false);
        
        *data_dir = 0xFF;
        
        return ret;
    } else {
        *data_dir &= ~(0xF0);
        
        setEnable(true);
        _delay_ms(LCD_STROBE_DURATION);

        unsigned char high = *data_pin & 0xF0;
        setEnable(false);
        
        setEnable(true);
        _delay_ms(LCD_STROBE_DURATION);

        unsigned char low = (*data_pin & 0xF0) >> 4;
        setEnable(false);
        
        *data_dir |= 0xF0;
        
        return (high | low);
    }
    
    return 0;
}

void LCD::busyWait() {
    while ((readBusyFlagAndAC() & 0x80) == 0x80) {}
}

void LCD::sendCommand(byte cmd) {
    busyWait();
    
    setRead(false);
    selectIR();

    if (_8bitmode) {
        *data_port = cmd;
        strobeEnable();
        *data_port = 0;
    } else {
        byte high = cmd & 0xF0;
        byte low = (cmd & 0x0F) << 4;
        
        *data_port &= ~(0xF0);
        *data_port |= high;
        strobeEnable();
        
        *data_port &= ~(0xF0);
        *data_port |= low;
        strobeEnable();
        
        *data_port &= ~(0xF0);
    }
}

void LCD::sendCharacter(char character) {
    busyWait();
    
    setRead(false);
    selectDR();

    if (_8bitmode) {
        *data_port = character;
        strobeEnable();
        *data_port = 0;
    } else {
        byte high = character & 0xF0;
        byte low = (character & 0x0F) << 4;
        
        *data_port &= ~(0xF0);
        *data_port |= high;
        strobeEnable();
        
        *data_port &= ~(0xF0);
        *data_port |= low;
        strobeEnable();
        
        *data_port &= ~(0xF0);
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
    sendCommand((1 << 5) | (_8bit << 4) | (_2line << 3) | (useBigFont << 2));
    
    _8bitmode = _8bit;

    if (!_8bit) {
        // re-send proper function set after initial one sent as 8-bit (necessary
        sendCommand((1 << 5) | (_8bit << 4) | (_2line << 3) | (useBigFont << 2));
    }
}

void LCD::setDRAMAddress(byte address) {
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
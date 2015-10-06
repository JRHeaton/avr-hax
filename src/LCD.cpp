#include "LCD.h"
#include <util/delay.h>

LCD::LCD(bool twoLineDisplay,
         bool useLargerFont,
         Pin *RS,
         Pin *RW,
         Pin *E,
         Pin *DB7,
         Pin *DB6,
         Pin *DB5,
         Pin *DB4,
         Pin *DB3,
         Pin *DB2,
         Pin *DB1,
         Pin *DB0) :
    twoLineDisplay(twoLineDisplay), useLargerFont(useLargerFont),
    RS(RS), RW(RW), E(E),
    DB7(DB7), DB6(DB6), DB5(DB5), DB4(DB4),
    DB3(DB3), DB2(DB2), DB1(DB1), DB0(DB0)
{
    RS->setMode(OUTPUT);
    RW->setMode(OUTPUT);
    E->setMode(OUTPUT);
    
    _8bitmode = (DB3 != NULL && DB2 != NULL && DB1 != NULL && DB0 != NULL);
    
    file.put = LCD::file_put;
    file.udata = this;
    file.flags = __SRD | __SWR;
}

void LCD::init() {
    _delay_ms(200); // max time specified by datasheet
    
    RW->write(LOW); // write
    RS->write(LOW); // IR
    
    setDBMode(OUTPUT);
    
    uint8_t function_set = (1 << 5) | (_8bitmode << 4) | (twoLineDisplay << 3) | (useLargerFont << 2);
    for (uint8_t i=0;i<3;++i) {
        writeDB(function_set | (1 << 4));
        pulseE();
        writeDB(LOW);
        _delay_ms(5);
    }
    if (!_8bitmode) {
        writeDB(function_set);
        pulseE();
        writeDB(LOW);
        _delay_ms(5);
        
        sendCommand(function_set, false);
    }
}

uint8_t LCD::readBusyFlagAndAC() {
    RW->write(HIGH); // read
    RS->write(LOW); // address + busy
    setDBMode(INPUT);
    
    uint8_t ret = 0;
    
    if (_8bitmode) {
        E->write(HIGH);
        _delay_us(LCD_PULSE_DURATION_MICROS);
        ret = readDB();
        E->write(LOW);
    } else {
        // Turn on E, wait
        E->write(HIGH);
        _delay_us(LCD_PULSE_DURATION_MICROS);

        // Read high 4 bits, turn off E
        uint8_t high = readDB();
        E->write(LOW);
        
        // Turn on E, wait
        E->write(HIGH);
        _delay_us(LCD_PULSE_DURATION_MICROS);

        // Read low 4 bits, turn off E
        uint8_t low = readDB() >> 4;
        E->write(LOW);

        ret = (high | low);
    }
    
    setDBMode(OUTPUT);
    
    return ret;
}

void LCD::busyWait() {
    while ((readBusyFlagAndAC() & 0x80) == 0x80) {}
}

void LCD::sendCommand(uint8_t cmd, bool busyWait) {
    if (busyWait) {
        this->busyWait();
    }
    
    RW->write(LOW); // write
    RS->write(LOW); // IR
//    E->write(LOW);

    if (_8bitmode) {
        writeDB(cmd);
        pulseE();
        writeDB(LOW);
    } else {
        uint8_t high = cmd & 0xF0;
        uint8_t low = (cmd & 0x0F) << 4;
        
        // Send high 4 bits
        writeDB(high);
        pulseE();
        
        // Send low 4 bits
        writeDB(low);
        pulseE();
        
        // Reset bus lines LOW
        writeDB(LOW);
    }
}

void LCD::sendCharacter(char character) {
    busyWait();
    
    RW->write(LOW); // write
    RS->write(HIGH); // DR
//    E->write(LOW);

    if (_8bitmode) {
        writeDB(character);
        pulseE();
        writeDB(LOW);
    } else {
        uint8_t high = character & 0xF0;
        uint8_t low = (character & 0x0F) << 4;
        
        // Send high 4 bits
        writeDB(high);
        pulseE();
        
        // Send low 4 bits
        writeDB(low);
        pulseE();
        
        // Reset bus lines LOW
        writeDB(LOW);
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

void LCD::setDBMode(uint8_t mode) {
    if (_8bitmode) {
        DB0->setMode(mode);
        DB1->setMode(mode);
        DB2->setMode(mode);
        DB3->setMode(mode);
    }
    DB4->setMode(mode);
    DB5->setMode(mode);
    DB6->setMode(mode);
    DB7->setMode(mode);
}

void LCD::writeDB(uint8_t value) {
    if (_8bitmode) {
        DB0->write(value & (1 << 0));
        DB1->write(value & (1 << 1));
        DB2->write(value & (1 << 2));
        DB3->write(value & (1 << 3));
    }
    DB4->write(value & (1 << 4));
    DB5->write(value & (1 << 5));
    DB6->write(value & (1 << 6));
    DB7->write(value & (1 << 7));
}

uint8_t LCD::readDB() {
    uint8_t ret = 0;
    if (_8bitmode) {
        ret |= DB0->read();
        ret |= DB1->read() << 1;
        ret |= DB2->read() << 2;
        ret |= DB3->read() << 3;
    }
    ret |= DB4->read() << 4;
    ret |= DB5->read() << 5;
    ret |= DB6->read() << 6;
    ret |= DB7->read() << 7;
    
    return ret;
}

void LCD::pulseE() {
    E->write(LOW);
    _delay_us(10);
    E->write(HIGH);
    _delay_us(LCD_PULSE_DURATION_MICROS);
    E->write(LOW);
    _delay_us(40);
}
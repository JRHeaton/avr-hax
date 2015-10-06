#ifndef LCD_H
#define LCD_H

#include <stdio.h>

#include "constants.h"
#include "Pin.h"
#include "Port.h"

#ifndef LCD_PULSE_DURATION_MICROS
#define LCD_PULSE_DURATION_MICROS 1000
#endif

class LCD {
public:
    
    LCD(bool twoLineDisplay,
        bool useLargerFont,
        Pin *RS,
        Pin *RW,
        Pin *E,
        Pin *DB7,
        Pin *DB6,
        Pin *DB5,
        Pin *DB4,
        Pin *DB3 = NULL,
        Pin *DB2 = NULL,
        Pin *DB1 = NULL,
        Pin *DB0 = NULL);
    
    // - Initialization by instruction
    void init();
    
    // - I/O
    void sendCommand(uint8_t cmd, bool busyWait=true);
    void sendCharacter(char character);
    void sendString(const char *string);
    
    // - High level commands
    void clearDisplay();
    void returnHome();
    void entryModeSet(bool increment, bool displayShift);
    void displayMode(bool displayOn,
                     bool cursorOn,
                     bool cursorBlink);
    void shift(bool followDisplayShift,
               bool directionRight);
    void setDRAMAddress(uint8_t address);
    uint8_t readBusyFlagAndAC();        // top bit is busy flag, low 7 are AC
    void busyWait();                    // wait until busy flag unset
    
    FILE file;
    
protected:
    
    static int file_put(char c, FILE *f);
    static int file_get(FILE *f);
    
    Pin *RS;
    Pin *RW;
    Pin *E;
    Pin *DB7;
    Pin *DB6;
    Pin *DB5;
    Pin *DB4;
    Pin *DB3;
    Pin *DB2;
    Pin *DB1;
    Pin *DB0;
    
    bool _8bitmode = true, twoLineDisplay, useLargerFont;
    
    void setDBMode(uint8_t mode);
    void writeDB(uint8_t value);
    uint8_t readDB();
    
    void pulseE();
};

#endif
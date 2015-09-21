#ifndef LCD_H
#define LCD_H

#include "constants.h"
#include <stdio.h>

#ifndef LCD_STROBE_DURATION
#define LCD_STROBE_DURATION 1
#endif

class LCD {
public:
    
    LCD(addr *data_dir, addr *data_port, addr *data_pin,
        addr *ctrl_dir, addr *ctrl_port,
        byte rs, byte rw, byte e);
    
    // - I/O
    void sendCommand(byte cmd);
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
    void functionSet(bool _8bit,
                     bool _2line,
                     bool useBigFont);
    void setDRAMAddress(byte address);
    byte readBusyFlagAndAC();           // top bit is busy flag, low 7 are AC
    void busyWait();                    // wait until busy flag unset
    
    FILE file;
    
protected:
    
    // - Basic control functions
    void selectDR();                    // select data reg (RS)
    void selectIR();                    // select inst reg (RS)
    void setRead(bool read);            // set RW direction
    void setEnable(bool enable);        // set E
    void strobeEnable();                // set E for LCD_STROBE_DURATION ms
    
    static int file_put(char c, FILE *f);
    static int file_get(FILE *f);
    
    addr *data_dir, *data_port, *data_pin;
    addr *ctrl_dir, *ctrl_port;
    byte rs, rw, e;
    
    bool _8bitmode = true;
};

#endif
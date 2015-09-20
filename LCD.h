#ifndef LCD_H
#define LCD_H

#include "constants.h"
#include <stdio.h>

class LCD {
public:
    
    LCD(addr *data_dir, addr *data_port, addr *data_pin,
        addr *ctrl_dir, addr *ctrl_port,
        byte rs, byte rw, byte e);
    
    void selectDR();
    void selectIR();
    
    void setRead(bool read);
    void setEnable(bool enable);
    
    void strobeEnable();
    unsigned char readBusyFlag();
    void busyWait();
    void sendCommand(unsigned char cmd);
    void sendCharacter(char character);
    void sendString(const char *string);
    
    void clearDisplay();
    void returnHome();
    void displayMode(bool displayOn, bool cursorOn, bool cursorBlink);
    void functionSet(bool _8bit, bool _2line, bool useBigFont);
    void setDRAMAddress(byte address);
    
    static int file_put(char c, FILE *f);
    
protected:
    addr *data_dir, *data_port, *data_pin;
    addr *ctrl_dir, *ctrl_port;
    byte rs, rw, e;
};

#endif
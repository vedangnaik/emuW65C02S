#ifndef EMUTUI_H
#define EMUTUI_H

#include <ncurses.h>
#include <iomanip>
#include <sstream>
#include <thread>
#include "W65C02S.h"

class emuTUI {
public:
    emuTUI(uint8_t* memory, W65C02S* mp);
    // ~emuTUI();

    void start();

private:
    uint8_t* memory;
    W65C02S* mp;
    std::thread tuiThread;

    WINDOW* stackWin;
    int stackWinLen;
    void formatStack();

    WINDOW* memoryWin;
    int memoryWinLen;
    void formatMemory();

    WINDOW* regWin;
    int regWinLen;
    int regWinHeight;
    void formatRegs();    
};

#endif
#ifndef MONITOR_H
#define MONITOR_H

#include <ncurses.h>
#include <iomanip>
#include <sstream>
#include <thread>
#include "W65C02S.h"

class Monitor {
public:
    Monitor(uint8_t* memory, W65C02S* mp, 
        unsigned int monitorHeight, unsigned int monitorWidth);
    // ~emuTUI();

    void start();

private:
    uint8_t* memory;
    W65C02S* mp;
    std::thread tuiThread;
    unsigned int monitorHeight;
    unsigned int monitorWidth;

    WINDOW* memoryWin;
    unsigned int memoryWinLen;
    void formatMemoryWin();

    WINDOW* stackWin;
    unsigned int stackWinLen;
    void formatStackWin();

    WINDOW* regWin;
    unsigned int regWinLen;
    unsigned int regWinHeight;
    void formatRegWin();  

    WINDOW* controlWin;
    unsigned int cntWinLen;
    unsigned int cntWinHeight;
    void formatControlWin();

    WINDOW* flagsWin;
    unsigned int flagsWinLen;
    unsigned int flagsWinHeight;
    void formatFlagsWin();

    // WINDOW* screenWin;
    // unsigned int screenWinLen;
    // unsigned int screenWinHeight;
    // void formatScreenWin();
};

#endif
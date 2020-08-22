#ifndef MONITOR_H
#define MONITOR_H

#include <ncurses.h>
#include <iomanip>
#include <sstream>
#include <thread>
#include <atomic>
#include "W65C02S.h"

class Monitor {
public:
    Monitor(uint8_t* memory, W65C02S* mp, 
        unsigned int monitorHeight, unsigned int monitorWidth,
        int offsetY, int offsetX);
    ~Monitor();

    void start();

private:
    // references and thread stuff
    uint8_t* memory;
    W65C02S* mp;
    std::thread tuiThread;
    std::atomic<bool> terminateFlag;
    
    // info related to the "window" in which the monitor will place the memory window, stack window, etc.
    unsigned int monitorHeight;
    unsigned int monitorWidth;
    int offsetY;
    int offsetX;

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
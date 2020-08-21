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

    WINDOW* memoryWin;
    int memoryWinLen;
    void formatMemoryWin();

    WINDOW* stackWin;
    int stackWinLen;
    void formatStackWin();

    WINDOW* regWin;
    int regWinLen;
    int regWinHeight;
    void formatRegWin();  

    WINDOW* controlWin;
    int cntWinLen;
    int cntWinHeight;
    void formatControlWin();

    WINDOW* flagsWin;
    int flagsWinLen;
    int flagsWinHeight;
    void formatFlagsWin();

    WINDOW* screenWin;
    int screenWinLen;
    int screenWinHeight;
    void formatScreenWin();
};

#endif
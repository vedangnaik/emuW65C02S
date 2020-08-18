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

    WINDOW* stackWin;
    WINDOW* memoryWin;

    std::thread tuiThread;

    void formatMemory();
    void formatStack();
};

#endif
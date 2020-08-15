#ifndef EMUTUI_H
#define EMUTUI_H

#include <ncurses.h>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include "W65C02S.h"

class emuTUI {
public:
    emuTUI(uint8_t* memory, W65C02S* mp);
    // ~emuTUI();

    void printStack(unsigned int winHeight, unsigned int posX, unsigned int posY);

private:
    uint8_t* memory;
    W65C02S* mp;

};

#endif
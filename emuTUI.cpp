#include "emuTUI.h"

emuTUI::emuTUI(uint8_t* memory, W65C02S* mp) {
    this->memory = memory;
    this->mp = mp;
    
    // init all ncurses stuff here
    initscr();
    // init all windows here
    this->memoryWin = newwin(LINES, 112, 0, COLS - 112);
    box(this->memoryWin, 0, 0);
    this->stackWin = newwin(LINES, 70, 0, COLS - 112 - 70);
    box(this->stackWin, 0, 0);
}

void emuTUI::start() {
    this->tuiThread = std::thread([this]() { 
        while(1) {
            this->formatMemory();
            this->formatStack();
        }
    });
}

void emuTUI::formatMemory() {
    for (int l = 0; l < MAX_MEMSIZE; l += 32) {
        std::stringstream line;
        line << std::setfill('0') << std::setw(5) 
                    << std::hex << l << " ";
        for (int subl = l; subl < l + 32; subl += 8) {
            line << " ";
            for (int b = subl; b < subl + 8; b++) {
                line << std::setfill('0') << std::setw(2) 
                    << std::hex << (int)this->memory[b] << " ";
            }
        }
        mvwprintw(this->memoryWin, 3+(l / 32), 3, line.str().c_str());
    }
    wrefresh(this->memoryWin);
}

void emuTUI::formatStack() {
    for (int i = 0; i < 32; i++) {
        std::stringstream line;
        for (int j = 0; j < 8; j++) {
            int addr = i + (j * 32);

            line << " ";
            line << std::setfill('0') << std::setw(2) 
                    << std::hex << (addr & 0x00FF);
            line << ": ";
            if (addr > this->mp->S) { // stack pointer
                line << std::setfill('0') << std::setw(2) 
                    << std::hex << (int)this->memory[0x0100 + addr];
            } else {
                line << "--";
            }
            line << " ";
        }
        mvwprintw(this->stackWin, 3+i, 3, line.str().c_str());
    }
    wrefresh(this->stackWin);
}
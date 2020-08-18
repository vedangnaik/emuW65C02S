#include "emuTUI.h"

emuTUI::emuTUI(uint8_t* memory, W65C02S* mp) {
    this->memory = memory;
    this->mp = mp;
    
    // init all ncurses stuff here
    initscr();

    // init all windows here
    // set dimensions of all windows first. The offsets will also use the same values, so do it now.
    this->memoryWinLen = 75 + 6 + 6;
    this->stackWinLen = 70;
    this->regWinLen = COLS - this->stackWinLen - this->memoryWinLen;
    this->regWinHeight = 5 + 6;
    // registers window
    this->regWin = newwin(this->regWinHeight, this->regWinLen, 
            0, 0);
    box(this->regWin, 0, 0);
    // memory window
    this->memoryWin = newwin(LINES, this->memoryWinLen, 
            0, COLS - this->memoryWinLen);
    box(this->memoryWin, 0, 0);
    // stack window
    this->stackWin = newwin(LINES, this->stackWinLen, 
            0, COLS - this->memoryWinLen - this->stackWinLen);
    box(this->stackWin, 0, 0);
    
    
}

void emuTUI::start() {
    this->tuiThread = std::thread([this]() { 
        while(1) {
            this->formatMemory();
            this->formatStack();
            this->formatRegs();
        }
    });
}

void emuTUI::formatMemory() {
    for (int l = 0; l < MAX_MEMSIZE; l += 24) {
        std::stringstream line;
        line << std::setfill('0') << std::setw(5) 
                    << std::hex << l << " ";
        for (int subl = l; subl < l + 24; subl += 8) {
            line << " ";
            for (int b = subl; b < subl + 8; b++) {
                line << std::setfill('0') << std::setw(2) 
                    << std::hex << (int)this->memory[b] << " ";
            }
        }
        mvwprintw(this->memoryWin, 3+(l / 24), 3, line.str().c_str());
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

void emuTUI::formatRegs() {
    mvwprintw(this->regWin, 3, 6, 
        "%-*s%.2x", this->regWinLen - 14, "Accumulator:", this->mp->A);
    mvwprintw(this->regWin, 4, 6, 
        "%-*s%.2x", this->regWinLen - 14, "X:", this->mp->X);
    mvwprintw(this->regWin, 5, 6, 
        "%-*s%.2x", this->regWinLen - 14, "Y:", this->mp->Y);
    mvwprintw(this->regWin, 6, 6, 
        "%-*s%.2x", this->regWinLen - 14, "Processor status:", this->mp->P);
    mvwprintw(this->regWin, 7, 6, 
        "%-*s%.2x", this->regWinLen - 14, "Stack pointer:", this->mp->S);

    wrefresh(this->regWin);
}
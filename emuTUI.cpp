#include "emuTUI.h"

emuTUI::emuTUI(uint8_t* memory, W65C02S* mp) {
    this->memory = memory;
    this->mp = mp;
    
    // init all ncurses stuff here
    initscr();

    // init all windows here
    // memory window
    this->memoryWinLen = 75 + 6 + 6;
    this->memoryWin = newwin(LINES, this->memoryWinLen, 
            0, COLS - this->memoryWinLen);
    box(this->memoryWin, 0, 0);
    // stack window
    this->stackWinLen = 70;
    this->stackWin = newwin(LINES, this->stackWinLen, 
            0, COLS - this->memoryWinLen - this->stackWinLen);
    box(this->stackWin, 0, 0);
    // registers window
    this->regWinHeight = 5 + 6;
    this->regWinLen = COLS - this->stackWinLen - this->memoryWinLen;
    this->regWin = newwin(this->regWinHeight, this->regWinLen, 
            0, 0);
    box(this->regWin, 0, 0);
    // control window
    this->cntWinHeight = 3 + 6;
    this->cntWinLen = this->regWinLen;
    this->controlWin = newwin(this->cntWinHeight, this->cntWinLen, 
            this->regWinHeight, 0);
    box(this->controlWin, 0, 0);
    wrefresh(this->controlWin);
}

void emuTUI::start() {
    this->tuiThread = std::thread([this]() { 
        while(1) {
            this->formatMemoryWin();
            this->formatStackWin();
            this->formatRegWin();
            this->formatControlWin();
        }
    });
}

void emuTUI::formatMemoryWin() {
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

void emuTUI::formatStackWin() {
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

void emuTUI::formatRegWin() {
    mvwprintw(this->regWin, 3, 3, 
        "%-*s%.2x", this->regWinLen - 8, "Accumulator:", this->mp->A);
    mvwprintw(this->regWin, 4, 3, 
        "%-*s%.2x", this->regWinLen - 8, "X:", this->mp->X);
    mvwprintw(this->regWin, 5, 3, 
        "%-*s%.2x", this->regWinLen - 8, "Y:", this->mp->Y);
    mvwprintw(this->regWin, 6, 3, 
        "%-*s%.2x", this->regWinLen - 8, "Processor status:", this->mp->P);
    mvwprintw(this->regWin, 7, 3, 
        "%-*s%.2x", this->regWinLen - 8, "Stack pointer:", this->mp->S);

    wrefresh(this->regWin);
}

void emuTUI::formatControlWin() {
    mvwprintw(this->controlWin, 3, 3, 
        "%-*s%.4x", this->cntWinLen - 10, "Program counter:", this->mp->PC);
    mvwprintw(this->controlWin, 4, 3, 
        "%-*s%.2x", this->cntWinLen - 8, "Instruction register:", this->mp->IR);
    uint16_t addr = (this->memory[0xFFFD] << 8) + this->memory[0xFFFC];
    mvwprintw(this->controlWin, 5, 3, 
        "%-*s%.4x", this->cntWinLen - 10, "Reset vector :", addr);

    wrefresh(this->controlWin);
}
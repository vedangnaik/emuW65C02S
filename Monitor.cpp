#include "Monitor.h"

Monitor::Monitor(uint8_t* memory, W65C02S* mp, 
        unsigned int monitorHeight, unsigned int monitorWidth,
        int offsetY, int offsetX) 
    {
    this->memory = memory;
    this->mp = mp;
    this->monitorHeight = monitorHeight;
    this->monitorWidth = monitorWidth;
    this->offsetY = offsetY;
    this->offsetX = offsetX;
    
    // init all ncurses stuff here
    initscr();
    if (has_colors()) {
        start_color();
    }

    // init all windows here
    // memory window
    // length calculation: each column has 8 2-digit hex characters (16) plus 7 spaces between the numbers (16+7) plus 1 space before and after the 1st and 8th hex character (16+7+2=25). There are two such columns (25+25). The address counter takes up 5 hex characters plus a space before and after (5+2=7). The border takes up 2 more spaces (25+25+7+2).
    this->memoryWinLen = 25 + 25 + 7 + 2;
    this->memoryWin = newwin(
        this->monitorHeight, 
        this->memoryWinLen, 
        this->offsetY + 0, 
        this->offsetX + this->monitorWidth - this->memoryWinLen
    );
    box(this->memoryWin, 0, 0);
    std::string memoryWinTitle = "MEMORY";
    mvwprintw(
        this->memoryWin, 
        1, 
        (this->memoryWinLen - memoryWinTitle.length()) / 2, 
        memoryWinTitle.c_str()
    );

    // stack window
    // length calculation: each column of 32 addresses takes up 2 chacaracters for the addess, one for the space, 2 for contents of that address, and 2 for one space on either side (2+1+2+2=7). There are eight such columns to span the full 256 bytes of stack space (8*7=56). The border takes up 2 more spaces (56+2).
    this->stackWinLen = 56 + 2;
    this->stackWin = newwin(
        this->monitorHeight, 
        this->stackWinLen, 
        this->offsetY + 0, 
        this->offsetX + this->monitorWidth - this->memoryWinLen - this->stackWinLen
    );
    box(this->stackWin, 0, 0);
    std::string stackWinTitle = "STACK";
    mvwprintw(
        this->stackWin, 
        1, 
        (this->stackWinLen - stackWinTitle.length()) / 2, 
        stackWinTitle.c_str()
    );

    // registers window
    // height calculation: 5 registers, each takes a line (5). Plus 3 lines of padding above and below (5+3+3).
    this->regWinHeight = 5 + 3 + 3;
    this->regWinLen = this->monitorWidth - this->stackWinLen - this->memoryWinLen;
    this->regWin = newwin(
        this->regWinHeight, 
        this->regWinLen, 
        this->offsetY + 0, 
        this->offsetX + 0
    );
    box(this->regWin, 0, 0);
    std::string regWinTitle = "REGISTERS";
    mvwprintw(
        this->regWin, 
        1, 
        (this->regWinLen - regWinTitle.length()) / 2, 
        regWinTitle.c_str()
    );

    // control window
    // height calculation: the program counter and instruction register each take a line (2). Plus 3 lines of padding above and below (2+3+3).
    this->cntWinHeight = 2 + 3 + 3;
    this->cntWinLen = this->regWinLen;
    this->controlWin = newwin(
        this->cntWinHeight, 
        this->cntWinLen, 
        this->offsetY + this->regWinHeight, 
        this->offsetX + 0
    );
    box(this->controlWin, 0, 0);
    std::string cntWinTitle = "CONTROL";
    mvwprintw(
        this->controlWin, 
        1, 
        (this->cntWinLen - cntWinTitle.length()) / 2, 
        cntWinTitle.c_str()
    );

    // flags window
    // height calculation: 7 flags, each takes a line (7). Plus 3 lines of padding above and below (7+3+3).
    this->flagsWinHeight = 7 + 3 + 3;
    this->flagsWinLen = this->regWinLen;
    this->flagsWin = newwin(
        this->flagsWinHeight, 
        this->flagsWinLen, 
        this->offsetY + this->regWinHeight + this->cntWinHeight, 
        this->offsetX + 0
    );
    box(this->flagsWin, 0, 0);
    std::string flagsWinTitle = "FLAGS";
    mvwprintw(
        this->flagsWin, 
        1, 
        (this->flagsWinLen - flagsWinTitle.length()) / 2, 
        flagsWinTitle.c_str()
    );
}

void Monitor::start() {
    this->tuiThread = std::thread([this]() { 
        while(1) {
            this->formatMemoryWin();
            this->formatStackWin();
            this->formatRegWin();
            this->formatControlWin();
            this->formatFlagsWin();
        }
    });
}

void Monitor::formatMemoryWin() {
    for (int l = 0; l < MAX_MEMSIZE; l += 16) {
        std::stringstream line;
        line << " " << std::setfill('0') << std::setw(5) 
                    << std::hex << l << " ";
        for (int subl = l; subl < l + 16; subl += 8) {
            line << " ";
            for (int b = subl; b < subl + 8; b++) {
                line << std::setfill('0') << std::setw(2) 
                    << std::hex << (int)this->memory[b] << " ";
            }
        }
        mvwprintw(this->memoryWin, 3+(l / 16), 1, line.str().c_str());
    }
    wrefresh(this->memoryWin);
}

void Monitor::formatStackWin() {
    for (int i = 0; i < 32; i++) {
        std::stringstream line;
        for (int j = 0; j < 8; j++) {
            int addr = i + (j * 32);

            line << " ";
            line << std::setfill('0') << std::setw(2) 
                    << std::hex << addr;
            line << " ";
            if (addr > this->mp->S) { // stack pointer
                line << std::setfill('0') << std::setw(2) 
                    << std::hex << (int)this->memory[0x0100 + addr];
            } else {
                line << "--";
            }
            line << " ";
        }
        mvwprintw(this->stackWin, 3+i, 1, line.str().c_str());
    }
    wrefresh(this->stackWin);
}

void Monitor::formatRegWin() {
    // The label will take up all the space minus two characters for the contents of the register (2), minus 2 spaces for padding on both sides (-2-2), minus 2 spaces for the border (-2-2-2)
    mvwprintw(this->regWin, 3, 2, 
        "%-*s%.2x", this->regWinLen -2-2-2, "Accumulator:", this->mp->A);
    mvwprintw(this->regWin, 4, 2, 
        "%-*s%.2x", this->regWinLen -2-2-2, "X:", this->mp->X);
    mvwprintw(this->regWin, 5, 2, 
        "%-*s%.2x", this->regWinLen -2-2-2, "Y:", this->mp->Y);
    mvwprintw(this->regWin, 6, 2, 
        "%-*s%.2x", this->regWinLen -2-2-2, "Processor status:", this->mp->makePfromFlags());
    mvwprintw(this->regWin, 7, 2, 
        "%-*s%.2x", this->regWinLen -2-2-2, "Stack pointer:", this->mp->S);

    wrefresh(this->regWin);
}

void Monitor::formatControlWin() {
    // The label's space is -4-2-2 here because the program counter contents take up 4 characters of hex.
    mvwprintw(this->controlWin, 3, 2, 
        "%-*s%.4x", this->cntWinLen -4-2-2, "Program counter:", this->mp->PC);
    mvwprintw(this->controlWin, 4, 2, 
        "%-*s%.2x", this->cntWinLen -2-2-2, "Instruction register:", this->mp->IR);
    uint16_t addr = (this->memory[0xFFFD] << 8) + this->memory[0xFFFC];

    wrefresh(this->controlWin);
}

void Monitor::formatFlagsWin() {
    // the label's space is -1-2-2 becuase the flag contents take up only one character, for 0 or 1.
    mvwprintw(this->flagsWin, 3, 2, 
        "%-*s%.1d", this->flagsWinLen -1-2-2, "Carry:", this->mp->C);
    mvwprintw(this->flagsWin, 4, 2, 
        "%-*s%.1d", this->flagsWinLen -1-2-2, "Zero:", this->mp->Z);
    mvwprintw(this->flagsWin, 5, 2, 
        "%-*s%.1d", this->flagsWinLen -1-2-2, "Interrupt disable:", this->mp->I);
    mvwprintw(this->flagsWin, 6, 2, 
        "%-*s%.1d", this->flagsWinLen -1-2-2, "Decimal mode:", this->mp->D);
    mvwprintw(this->flagsWin, 7, 2, 
        "%-*s%.1d", this->flagsWinLen -1-2-2, "BRK:", this->mp->B);
    mvwprintw(this->flagsWin, 8, 2, 
        "%-*s%.1d", this->flagsWinLen -1-2-2, "Overflow:", this->mp->V);
    mvwprintw(this->flagsWin, 9, 2, 
        "%-*s%.1d", this->flagsWinLen -1-2-2, "Negative:", this->mp->N);

    wrefresh(this->flagsWin);
}
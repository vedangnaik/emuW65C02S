#include "emuTUI.h"

emuTUI::emuTUI(uint8_t* memory, W65C02S* mp) {
    this->memory = memory;
    this->mp = mp;
    // init all ncurses stuff here
    initscr();
}

// void printRAM() {
//     initscr();
//     WINDOW* memoryWin;
//     memoryWin = newwin(LINES, 112, 0, COLS - 112);
//     box(memoryWin, 0, 0);

//     while (1) {
//         for (int l = 0; l < MAX_MEMSIZE; l += 32) {
//             std::stringstream line;
//             line << std::setfill('0') << std::setw(5) 
//                         << std::hex << l << " ";
//             for (int subl = l; subl < l + 32; subl += 8) {
//                 line << " ";
//                 for (int b = subl; b < subl + 8; b++) {
//                     line << std::setfill('0') << std::setw(2) 
//                         << std::hex << (int)RAM[b] << " ";
//                 }
//             }
//             mvwprintw(memoryWin, 3+(l / 32), 3, line.str().c_str());
//         }
//         wrefresh(memoryWin);
//         std::this_thread::sleep_for(std::chrono::milliseconds(1));
//     }
//     delwin(memoryWin);
//     endwin();
// }

void emuTUI::printStack(unsigned int winHeight, unsigned int posX, unsigned int posY) {
    int winLen = 70;
    int colLen = 32;

    WINDOW* stackWin;
    stackWin = newwin(winHeight, winLen, 0, 0);
    box(stackWin, 0, 0);

    while(1) {
        for (int i = 0; i < colLen; i++) {
            std::stringstream line;
            for (int j = 0; j < 8; j++) {
                int addr = 0x0100 + i + (j * colLen);

                line << " ";
                line << std::setfill('0') << std::setw(2) 
                        << std::hex << (addr & 0x00FF);
                line << ": ";
                if (addr < this->mp->S) { // stack pointer
                    line << "--";
                } else {
                    line << std::setfill('0') << std::setw(2) 
                        << std::hex << (int)this->memory[addr];
                }
                line << " ";
            }
            mvwprintw(stackWin, 3+i, 3, line.str().c_str());
        }
        wrefresh(stackWin);
    }

    delwin(stackWin);
}
#include <iostream>
#include <fstream>
#include "W65C02S.h"
#include "emuTUI.h"

uint8_t RAM[MAX_MEMSIZE];


int main(int argc, char* argv[]) {
    // Check for correct number of args
    // (disabled for dev)
    // if (argc <= 1) {
    //     std::cout << "file name not provided" << std::endl; 
    //     return -1; 
    // }
    
    // get filename from args
    // (set to absolute path for dev)
    char* fileName = argv[1];
    std::ifstream f("/home/vedang/Desktop/asm6502/a.out", std::ios::binary);
    if (!f) {
        std::cout << "file open failed" << std::endl;
        return -1;
    }
    // This should be rewritten to properly align and pad files which are not exactly 65536 bytes long. 
    if (!f.read((char*)RAM, MAX_MEMSIZE)) {
        std::cout << "file read failed" << std::endl;
        return -1;
    }

    W65C02S* my6502 = new W65C02S(&RAM[0]);
    emuTUI* tui = new emuTUI(&RAM[0], my6502);

    tui->start();
    my6502->run();
}
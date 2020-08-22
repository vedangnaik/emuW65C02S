#include <iostream>
#include <fstream>
#include "W65C02S.h"
#include "Monitor.h"


int main(int argc, char* argv[]) {
    uint8_t memory[MAX_MEMSIZE];

    // Check for correct number of args
    // (disabled for dev)
    // if (argc <= 1) {
    //     std::cout << "file name not provided" << std::endl; 
    //     return -1; 
    // }
    
    // get filename from args
    // (set to absolute path for dev)
    char* fileName = argv[1];
    std::ifstream fs("/home/vedang/Desktop/AllSuiteA.bin", std::ios::binary);
    if (!fs) {
        std::cout << "file open failed" << std::endl;
        return -1;
    }
    // This should be rewritten to properly align and pad files which are not exactly 65536 bytes long.
    fs.seekg(0, fs.end);
    int fileSize = fs.tellg();
    fs.seekg(0, fs.beg);
    if (!fs.read((char*)(memory + MAX_MEMSIZE - fileSize), fileSize)) {
        std::cout << "file read failed" << std::endl;
        return -1;
    }

    W65C02S* my6502 = new W65C02S(&memory[0]);
    Monitor* tui = new Monitor(&memory[0], my6502, 43, 145, 0, 0);

    tui->start();
    my6502->run();
}
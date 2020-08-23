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
    std::ifstream fs("/home/vedang/Desktop/asm6502/a.out", std::ios::binary);
    // std::ifstream fs("/home/vedang/Desktop/AllSuiteA.bin", std::ios::binary);
    if (!fs) {
        std::cout << "file open failed" << std::endl;
        return -1;
    }
    // This may or may not be the best way to handle incorrect file sizes, but until it fails we'll see
    fs.seekg(0, fs.end);
    int fileSize = fs.tellg();
    fs.seekg(0, fs.beg);
    if (!fs.read((char*)(memory + MAX_MEMSIZE - fileSize), fileSize)) {
        std::cout << "file read failed" << std::endl;
        return -1;
    }

    W65C02S* my6502 = new W65C02S(&memory[0]);
    Monitor* monitor = new Monitor(&memory[0], my6502, 43, 145, 0, 0);

    monitor->start();
    my6502->run();

    // the run function will run infinitely if there are no errors. If an error occurs, say for example a invalid instruction or something, the function terminates. In that case, a large delay is introduced to give the user time to read the screen, then the custom destructor for Monitor is called, which terminates all threads and windows and shut down ncurses mode.
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    monitor->~Monitor();
}
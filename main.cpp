#include <iostream>
#include "W65C02S.h"

int main() {
    uint8_t RAM[65536] = {};
    RAM[0x00] = 0xea;
    RAM[0x01] = 0xea;
    RAM[0xFFFC] = 0x00;
    RAM[0xFFFD] = 0x00;

    std::cout << "Resetting 6502..." << std::endl;
    W65C02S* my6502 = new W65C02S(1, &RAM[0]);
    my6502->run();
}
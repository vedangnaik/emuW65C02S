#include <iostream>
#include "W65C02S.h"

int main() {
    // delcare and reset RAM
    uint8_t RAM[65536] = {};
    RAM[0xFFFC] = 0x00;
    RAM[0xFFFD] = 0x00;
    // program
    RAM[0x00] = 0xa9;
    RAM[0x01] = 0x42;
    RAM[0x02] = 0x8d;
    RAM[0x03] = 0x00;
    RAM[0x04] = 0x80;

    std::cout << "Resetting 6502..." << std::endl;
    W65C02S* my6502 = new W65C02S(1, &RAM[0]);
    my6502->run();
}
#include "W65C02S.h"

W65C02S::W65C02S(uint8_t* RAMPtr) {
    this->RAMPtr = RAMPtr;
    this->reset();
}

void W65C02S::reset() {
    this->A = this->X = this->Y = this->P = this->S = 0;
    this->PC = (this->RAMPtr[0xFFFD] << 8) + this->RAMPtr[0xFFFC];
}

void W65C02S::run() {
    while (this->PC < 65536) {
        // fetch instruction
        uint16_t nextAddress = this->PC;
        this->IR = this->RAMPtr[nextAddress];

        // decode instruction
        switch (this->IR) {
            case OPCODES::NOP:
                std::cout << "NOP" << std::endl;
                break;
            default:
                break;
        }

        // increment PC
        this->PC++;
    }
}

void W65C02S::printRegisters() {
    std::cout << "A: " << int(this->A) << std::endl;
    // TODO, add rest
}
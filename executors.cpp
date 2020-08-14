#include "W65C02S.h"

void W65C02S::NOP(uint8_t opcode) {
    ;
}

void W65C02S::LDA(uint8_t opcode) {
    if (opcode == 0xa9) {
        this->A = this->memory[++this->PC];
    }
    std::cout << "A: " << (int)this->A << std::endl;
}

void W65C02S::STA(uint8_t opcode) {
    if (opcode == 0x8d) {
        int addr = this->memory[++this->PC];
        addr += this->memory[++this->PC] << 8;
        this->memory[addr] = this->A;
    }
}

void W65C02S::JMP(uint8_t opcode) {
    if (opcode == 0x4c) {
        int addr = this->memory[++this->PC];
        addr += this->memory[++this->PC] << 8;
        this->PC = addr;
    }
}
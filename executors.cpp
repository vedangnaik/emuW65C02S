#include "W65C02S.h"

void W65C02S::NOP(uint8_t opcode) {
    ;
}

void W65C02S::LDA(uint8_t opcode) {
    if (opcode == 0xa9) {
        this->A = this->memory[++this->PC];
    }
}

void W65C02S::STA(uint8_t opcode) {
    if (opcode == 0x8d) {
        int addr = this->memory[++this->PC];
        addr += this->memory[++this->PC] << 8;
        this->memory[addr] = this->A;
    }
}
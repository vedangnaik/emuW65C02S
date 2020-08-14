#include "W65C02S.h"

void W65C02S::JMP(uint8_t opcode) {
    switch (opcode) {
        case 0x4C: {   // jump to absolute address
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            // The -1 is because the main loop will increment the PC by one, to the correct position.
            this->PC = addr - 1;
            break;
        } case 0x7C: {
            break; 
        } case 0x6C: {
            break; 
        } default: {
            std::cout << "invalid opcode for JMP" << std::endl;
        }
    }
}

void W65C02S::LDA(uint8_t opcode) {
    switch (opcode) {
        case 0xAD: { 
            break;
        } case 0xBD: {
            break; 
        } case 0xB9: {
            break; 
        } case 0xA9: { // immediate load
            this->A = this->memory[++this->PC];
            break; 
        } case 0xA5: {
            break; 
        } case 0xA1: {
            break; 
        } case 0xB5: {
            break; 
        } case 0xB2: {
            break; 
        } case 0xB1: {
            break; 
        } default: {
            std::cout << "invalid opcode for LDA" << std::endl;
        }
    }
}

void W65C02S::LDX(uint8_t opcode) {
    switch (opcode) {
        case 0xAE: {   // absolute address load
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->X = this->memory[addr]; 
            break;
        } case 0xBE: {
            break; 
        } case 0xA2: { // immediate load
            this->X = this->memory[++this->PC];
            break; 
        } case 0xA6: {
            break; 
        } case 0xB6: {
            break; 
        } default: {
            std::cout << "invalid opcode for LDX" << std::endl;
        }
    }
}

void W65C02S::LDY(uint8_t opcode) {
    switch (opcode) {
        case 0xAC: {   // absolute address load
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->Y = this->memory[addr]; 
            break;
        } case 0xBC: {
            break; 
        } case 0xA0: { // immediate load
            this->Y = this->memory[++this->PC];
            break; 
        } case 0xA4: {
            break; 
        } case 0xB4: {
            break; 
        } default: {
            std::cout << "invalid opcode for LDY" << std::endl;
        }
    }
}

void W65C02S::NOP(uint8_t opcode) {
    ;
}

void W65C02S::ROL(uint8_t opcode) {
    switch (opcode) {
        case 0x2E: {   // absolute rotate left 
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            uint8_t value = this->memory[addr];
            this->memory[addr] = ((value & 0x80) >> 7) | (value << 1);
            break;
        } case 0x3E: {
            break; 
        } case 0x2A: { // register A rotate left
            uint8_t value = this->A;
            this->A = ((value & 0x80) >> 7) | (value << 1);
            break; 
        } case 0x26: { 
            break; 
        } case 0x36: {
            break; 
        } default: {
            std::cout << "invalid opcode for ROL" << std::endl;
        }
    }
}

void W65C02S::ROR(uint8_t opcode) {
    switch (opcode) {
        case 0x6E: {   // absolute rotate left 
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            uint8_t value = this->memory[addr];
            this->memory[addr] = ((value & 0x01) << 7) | (value >> 1);
            break;
        } case 0x7E: {
            break; 
        } case 0x6A: { // register A rotate left
            uint8_t value = this->A;
            this->A = ((value & 0x01) << 7) | (value >> 1);
            break; 
        } case 0x66: { 
            break; 
        } case 0x76: {
            break; 
        } default: {
            std::cout << "invalid opcode for ROR" << std::endl;
        }
    }
}

void W65C02S::STA(uint8_t opcode) {
    switch (opcode) {
        case 0x8D: {   // store at absolute address
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->memory[addr] = this->A;
            break;
        } case 0x9D: {
            break; 
        } case 0x99: {
            break; 
        } case 0xA9: { 
            break; 
        } case 0x85: {
            break; 
        } case 0x81: {
            break; 
        } case 0x95: {
            break; 
        } case 0x92: {
            break; 
        } case 0x91: {
            break; 
        } default: {
            std::cout << "invalid opcode for STA" << std::endl;
        }
    }
}

void W65C02S::STX(uint8_t opcode) {
    switch (opcode) {
        case 0x8E: {   // store at absolute address
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->memory[addr] = this->X;
            break;
        } case 0x86: {
            break; 
        } case 0x96: {
            break; 
        } default: {
            std::cout << "invalid opcode for STX" << std::endl;
        }
    }
}

void W65C02S::STY(uint8_t opcode) {
    switch (opcode) {
        case 0x8C: {   // store at absolute address
            int addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->memory[addr] = this->Y;
            break;
        } case 0x84: {
            break; 
        } case 0x94: {
            break; 
        } default: {
            std::cout << "invalid opcode for STY" << std::endl;
        }
    }
}
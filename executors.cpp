#include "W65C02S.h"

/*  ADDRESSING MODE CALCULATORS
    addr is always the address of the operand. This address is calculated by whatever means neccessary depending on the particular addressing mode. 
*/
uint16_t W65C02S::abs() {
    uint16_t addr = this->memory[++this->PC];
    addr += this->memory[++this->PC] << 8;
    return addr;
}
uint16_t W65C02S::absIndX() {
    uint16_t addr = this->memory[++this->PC];
    addr += this->memory[++this->PC] << 8;
    addr += this->X;
    return addr;
}
uint16_t W65C02S::absIndY() {
    uint16_t addr = this->memory[++this->PC];
    addr += this->memory[++this->PC] << 8;
    addr += this->Y;
    return addr;
}
uint16_t W65C02S::zp() {
    uint16_t addr = this->memory[++this->PC];
    return addr;
}
uint16_t W65C02S::zpIndIndir() {
    uint16_t lowByteAddr = (this->memory[++this->PC] + this->X) & 0xFF;
    uint16_t addr = this->memory[lowByteAddr];
    addr += this->memory[++lowByteAddr] << 8;
    return addr;
}
uint16_t W65C02S::zpIndX() {
    uint16_t addr = (this->memory[++this->PC] + this->X) & 0xFF;
    return addr;
}
uint16_t W65C02S::zpIndY() {
    uint16_t addr = (this->memory[++this->PC] + this->Y) & 0xFF;
    return addr;
}
uint16_t W65C02S::zpIndir() {
    uint16_t lowByteAddr = this->memory[++this->PC];
    uint16_t addr = this->memory[lowByteAddr];
    addr += this->memory[++lowByteAddr] << 8;
    return addr;
}
uint16_t W65C02S::zpIndirIndY() {
    uint16_t lowByteAddr = this->memory[++this->PC];
    uint16_t addr = this->memory[lowByteAddr];
    addr += this->memory[++lowByteAddr] << 8;
    addr += this->Y;
    return addr;
}


/*  INSTRUCTION EXECUTORS
    The instructions are ordered alphabetically the same way they are presented in the official datasheet from WDC.
*/
void W65C02S::ADC(uint8_t opcode) {
    // Values needed to calculate the flags
    // A = oldA + operand + carry
    uint8_t oldA = this->A;
    uint8_t operand;
    switch (opcode) { // a
        case 0x6D: {
            operand = this->memory[abs()];
            break;
        } case 0x7D: { // a, x
            operand = this->memory[absIndX()];
            break;
        } case 0x79: { // a, y
            operand = this->memory[absIndY()];
            break;
        } case 0x69: { // #
            operand = this->memory[++this->PC];
            break;
        } case 0x65: { // zp
            operand = this->memory[zp()];
            break;
        } case 0x61: { // (zp, x)
            operand = this->memory[zpIndIndir()];
            break;
        } case 0x75: { // zp, x
            operand = this->memory[zpIndX()];
            break; 
        } case 0x72: { // (zp)
            operand = this->memory[zpIndir()];
            break; 
        } case 0x71: { // (zp), y
            operand = this->memory[zpIndirIndY()];
            break; 
        } default: {
            std::cout << "invalid opcode for ADC" << std::endl;
        }
    }
    // calculate sum and store in A register
    this->A = oldA + operand + this->C;
    // set flags
    this->C = this->A > 255;
    this->Z = this->A == 0;
    this->V = (bool)((oldA ^ this->A) & (operand ^ this->A) & 0x80); 
    this->N = (bool)(this->A & 0x80);
}

void W65C02S::AND(uint8_t opcode) {
    uint8_t operand;
    switch (opcode) { 
        case 0x2D: { // a
            operand = this->memory[abs()];
            break;
        } case 0x3D: { // a, x
            operand = this->memory[absIndX()];
            break; 
        } case 0x39: { // a, y
            operand = this->memory[absIndY()];
            break; 
        } case 0x29: { // #
            operand = this->memory[++this->PC];
            break; 
        } case 0x25: { // zp
            operand = this->memory[zp()];
            break; 
        } case 0x21: { // (zp, x)
        operand = this->memory[zpIndIndir()];
            break; 
        } case 0x35: { // zp, x
            operand = this->memory[zpIndX()];
            break; 
        } case 0x32: { // (zp)
            operand = this->memory[zpIndir()];
            break; 
        } case 0x31: { // (zp), y
            operand = this->memory[zpIndirIndY()];
            break;
        } default: {
            std::cout << "invalid opcode for AND" << std::endl;
        }
    }
    this->A = this->A & operand;
    this->Z = this->A == 0;
    this->N = (bool)(this->A & 0x80);
}

void W65C02S::BCC(uint8_t opcode) {
    switch (opcode) {
        case 0x90: { // r
            uint8_t offset = this->memory[++this->PC];
            if (!this->C) {
                this->PC += offset;
            }
            break;
        } default: {
            std::cout << "invalid opcode for BCC" << std::endl;
        }
    }
}

void W65C02S::BCS(uint8_t opcode) {
    switch (opcode) {
        case 0xB0: { // r
            uint8_t offset = this->memory[++this->PC];
            if (this->C) {
                this->PC += offset;
            }
            break;
        } default: {
            std::cout << "invalid opcode for BCS" << std::endl;
        }
    }
}

void W65C02S::BEQ(uint8_t opcode) {
    switch (opcode) {
        case 0xF0: { // r
            uint8_t offset = this->memory[++this->PC];
            if (this->Z) {
                this->PC += offset;
            }
            break;
        } default: {
            std::cout << "invalid opcode for BEQ" << std::endl;
        }
    }
}

void W65C02S::CLC(uint8_t opcode) {
    switch (opcode) {
        case 0x18: { // i
            this->C = false;
            break;
        } default: {
            std::cout << "invalid opcode for CLC" << std::endl;
        }
    }
}

void W65C02S::CLD(uint8_t opcode) {
    switch (opcode) { // i
        case 0xD8: {
            this->D = false;
            break;
        } default: {
            std::cout << "invalid opcode for CLD" << std::endl;
        }
    }
}

void W65C02S::CLI(uint8_t opcode) {
    switch (opcode) {
        case 0x58: { // i
            this->I = false;
            break;
        } default: {
            std::cout << "invalid opcode for CLI" << std::endl;
        }
    }
}

void W65C02S::CLV(uint8_t opcode) {
    switch (opcode) {
        case 0xB8: { // i
            this->V = false;
            break;
        } default: {
            std::cout << "invalid opcode for CLV" << std::endl;
        }
    }
}

void W65C02S::CMP(uint8_t opcode) {
    uint8_t operand;
    switch (opcode) {
        case 0xCD: { // a
            operand = this->memory[abs()];
            break;
        } case 0xDD: { // a, x
            operand = this->memory[absIndX()];
            break; 
        } case 0xD9: { // a, y
            operand = this->memory[absIndY()];
            break; 
        } case 0xC9: { // #
            operand = this->memory[++this->PC];
            break; 
        } case 0xC5: { // zp
            operand = this->memory[zp()];
            break;
        } case 0xC1: { // (zp, x)
            operand = this->memory[zpIndIndir()];
            break; 
        } case 0xD5: { // zp, x
            operand = this->memory[zpIndX()];
            break; 
        } case 0xD2: { // (zp)
            operand = this->memory[zpIndir()];
            break; 
        } case 0xD1: { // (zp), y
            operand = this->memory[zpIndirIndY()];
            break; 
        } default: {
            std::cout << "invalid opcode for CMP" << std::endl;
        }
    }
    // Internally, CMP subtracts the operand from the accumulator without borrow, uses the result to set the flags, then discards it.
    uint16_t res = this->A - operand;
    // the flags are set in the same was as SBC does.
    this->C = res <= 255;
    this->Z = res == 0;
    this->N = (bool)(res & 0x80);
    // res is discarded, leaving all memory locations and registers untouched.
}

void W65C02S::CPX(uint8_t opcode) {
    uint8_t operand;
    switch (opcode) {
        case 0xEC: { // a
            operand = this->memory[abs()];
            break;
        } case 0xE0: { // #
            operand = this->memory[++this->PC];
            break;
        } case 0xE4: { // zp
            operand = this->memory[zp()];
            break;
        } default: {
            std::cout << "invalid opcode for CPX" << std::endl;
        }
    }
    uint16_t res = this->X - operand;
    this->C = res <= 255;
    this->Z = res == 0;
    this->N = (bool)(res & 0x80);
}

void W65C02S::CPY(uint8_t opcode) {
    uint8_t operand;
    switch (opcode) {
        case 0xCC: { // a
            operand = this->memory[abs()];
            break;
        } case 0xC0: { // #
            operand = this->memory[++this->PC];
            break;
        } case 0xC4: { // zp
            operand = this->memory[zp()];
            break;
        } default: {
            std::cout << "invalid opcode for CPY" << std::endl;
        }
    }
    uint16_t res = this->Y - operand;
    this->C = res <= 255;
    this->Z = res == 0;
    this->N = (bool)(res & 0x80);
}

void W65C02S::DEC(uint8_t opcode) {
    uint8_t res;
    switch (opcode) {
        case 0xCE: { // a
            res = --this->memory[abs()];
            break;
        } case 0xDE: { // a, x
            res = --this->memory[absIndX()];
            break; 
        } case 0x3A: { // A
            res = --this->A;
            break; 
        } case 0xC6: { // zp
            res = --this->memory[zp()];
            break; 
        } case 0xD6: { // zp, x
            res = --this->memory[zpIndX()];
            break;
        } default: {
            std::cout << "invalid opcode for DEC" << std::endl;
        }
    }
    this->Z = res == 0;
    this->N = (bool)(res & 0x80);
}

void W65C02S::DEX(uint8_t opcode) {
    switch (opcode) {
        case 0xCA: { // i
            this->X--;
            break;
        } default: {
            std::cout << "invalid opcode for DEX" << std::endl;
        }
    }
    this->Z = this->X == 0;
    this->N = (bool)(this->X & 0x80);
}

void W65C02S::DEY(uint8_t opcode) {
    switch (opcode) { // i
        case 0x88: {
            this->Y--;
            break;
        } default: {
            std::cout << "invalid opcode for DEY" << std::endl;
        }
    }
    this->Z = this->Y == 0;
    this->N = (bool)(this->Y & 0x80);
}

void W65C02S::INC(uint8_t opcode) {
    uint8_t res;
    switch (opcode) {
        case 0xEE: { // a
            res = ++this->memory[abs()];
            break;
        } case 0xFE: { // a, x
            res = ++this->memory[absIndX()];
            break; 
        } case 0x1A: { // A
            res = ++this->A;
            break; 
        } case 0xE6: { // zp
            res = ++this->memory[zp()];
            break; 
        } case 0xF6: { // zp, x
            res = ++this->memory[zpIndX()];
            break; 
        } default: {
            std::cout << "invalid opcode for INC" << std::endl;
        }
    }
    this->Z = res == 0;
    this->N = (bool)(res & 0x80);
}

void W65C02S::INX(uint8_t opcode) {
    switch (opcode) {
        case 0xE8: { // i
            this->X++;
            break;
        } default: {
            std::cout << "invalid opcode for INX" << std::endl;
        }
    }
    this->Z = this->X == 0;
    this->N = (bool)(this->X & 0x80);
}

void W65C02S::INY(uint8_t opcode) {
    switch (opcode) {
        case 0xC8: { // i
            this->Y++;
            break;
        } default: {
            std::cout << "invalid opcode for INY" << std::endl;
        }
    }
    this->Z = this->Y == 0;
    this->N = (bool)(this->Y & 0x80);
}

void W65C02S::JMP(uint8_t opcode) {
    // The -1 is because the main loop will increment the PC by one after executing this function, to the correct position.
    switch (opcode) {
        case 0x4C: { // a
            this->PC = abs() - 1;
            break;
        } case 0x7C: { // (a, x)
            uint16_t ptrAddr = this->memory[++this->PC];
            ptrAddr += this->memory[++this->PC] << 8;
            ptrAddr += this->X;
            uint16_t jmpAddr = this->memory[ptrAddr];
            jmpAddr += this->memory[++ptrAddr] << 8;
            this->PC = jmpAddr - 1;
            break;
        } case 0x6C: { // (a)
            uint16_t ptrAddr = this->memory[++this->PC];
            ptrAddr += this->memory[++this->PC] << 8;
            uint16_t jmpAddr = this->memory[ptrAddr];
            jmpAddr += this->memory[++ptrAddr] << 8;
            this->PC = jmpAddr - 1;
            break;
        } default: {
            std::cout << "invalid opcode for JMP" << std::endl;
        }
    }
}

void W65C02S::JSR(uint8_t opcode) {
    switch (opcode) {
        case 0x20: { // a
            int subroutineAddr = abs();
            // push current PC value to stack (this is the point to return to)
            // the 6502 reportedly pushed the location of the next address - 1 to the stack. Here, there's no need to do - 1 since the PC hasn't been incremented; the next instruction is at at PC++, which the main loop will do after RTS loads this value back into the PC.
            this->memory[0x0100 + (this->S--)] = this->PC >> 8;
            this->memory[0x0100 + (this->S--)] = this->PC & 0x00FF;
            // As with JMP, the - 1 is because the main loop will increment PC.
            this->PC = subroutineAddr - 1;
            break;
        } default: {
            std::cout << "invalid opcode for JSR" << std::endl;
        }
    }
}

void W65C02S::LDA(uint8_t opcode) {
    switch (opcode) {
        case 0xAD: {
            uint16_t addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->A = this->memory[addr];
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
        } case 0xB5: { // zero page indirect address with register X
            uint16_t addr = this->X + this->memory[++this->PC];
            this->A = this->memory[addr];
            break;
        } case 0xB2: {
            break;
        } case 0xB1: {
            break;
        } default: {
            std::cout << "invalid opcode for LDA" << std::endl;
        }
    }
    this->Z = this->A == 0;
    this->N = (bool)(this->A & 0x80);
}

void W65C02S::LDX(uint8_t opcode) {
    switch (opcode) {
        case 0xAE: {   // absolute address load
            uint16_t addr = this->memory[++this->PC];
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
    this->Z = this->X == 0;
    this->N = (bool)(this->X & 0x80);
}

void W65C02S::LDY(uint8_t opcode) {
    switch (opcode) {
        case 0xAC: {   // absolute address load
            uint16_t addr = this->memory[++this->PC];
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
    this->Z = this->Y == 0;
    this->N = (bool)(this->Y & 0x80);
}

void W65C02S::NOP(uint8_t opcode) {
    ;
}

void W65C02S::PHA(uint8_t opcode) {
    switch (opcode) {
        case 0x48: {
            this->memory[0x0100 + this->S] = this->A;
            this->S--;
            break;
        } default: {
            std::cout << "invalid opcode for PHA" << std::endl;
        }
    }
}

void W65C02S::PHP(uint8_t opcode) {
    switch (opcode) {
        case 0x08: {
            this->memory[0x0100 + this->S] = this->makePfromFlags();
            this->S--;
            break;
        } default: {
            std::cout << "invalid opcode for PHP" << std::endl;
        }
    }
}

void W65C02S::PHX(uint8_t opcode) {
    switch (opcode) {
        case 0xDA: {
            this->memory[0x0100 + this->S] = this->X;
            this->S--;
            break;
        } default: {
            std::cout << "invalid opcode for PHX" << std::endl;
        }
    }
}

void W65C02S::PHY(uint8_t opcode) {
    switch (opcode) {
        case 0x5A: {
            this->memory[0x0100 + this->S] = this->Y;
            this->S--;
            break;
        } default: {
            std::cout << "invalid opcode for PHY" << std::endl;
        }
    }
}

void W65C02S::PLA(uint8_t opcode) {
    switch (opcode) {
        case 0x68: {
            this->S++;
            this->A = this->memory[0x0100 + this->S];
            break;
        } default: {
            std::cout << "invalid opcode for PLA" << std::endl;
        }
    }
    this->Z = this->A == 0;
    this->N = (bool)(this->A & 0x80);
}

void W65C02S::PLP(uint8_t opcode) {
    switch (opcode) {
        case 0x28: {
            this->S++;
            this->setFlagsFromP(this->memory[0x0100 + this->S]);
            break;
        } default: {
            std::cout << "invalid opcode for PLP" << std::endl;
        }
    }
}

void W65C02S::PLX(uint8_t opcode) {
    switch (opcode) {
        case 0xFA: {
            this->S++;
            this->X = this->memory[0x0100 + this->S];
            break;
        } default: {
            std::cout << "invalid opcode for PLX" << std::endl;
        }
    }
    this->Z = this->X == 0;
    this->N = (bool)(this->X & 0x80);
}

void W65C02S::PLY(uint8_t opcode) {
    switch (opcode) {
        case 0x7A: {
            this->S++;
            this->Y = this->memory[0x0100 + this->S];
            break;
        } default: {
            std::cout << "invalid opcode for PLY" << std::endl;
        }
    }
    this->Z = this->Y == 0;
    this->N = (bool)(this->Y & 0x80);
}

void W65C02S::ROL(uint8_t opcode) {
    uint8_t res;
    switch (opcode) {
        case 0x2E: {   // absolute rotate left 
            uint16_t addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->C = (bool)(this->memory[addr] & 0x80);
            res = (this->memory[addr] << 1) | this->C;
            this->memory[addr] = res;
            break;
        } case 0x3E: {
            break;
        } case 0x2A: { // register A rotate left
            this->C = (bool)(this->A & 0x80);
            res = (this->A << 1) | this->C;
            this->A = res;
            break;
        } case 0x26: { 
            break;
        } case 0x36: {
            break;
        } default: {
            std::cout << "invalid opcode for ROL" << std::endl;
        }
    }
    this->Z = res == 0;
    this->N = (bool)(res & 0x80);
}

void W65C02S::ROR(uint8_t opcode) {
    uint8_t res;
    switch (opcode) {
        case 0x6E: {   // absolute rotate left 
            uint16_t addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            this->C = (bool)(this->memory[addr] & 0x01);
            res = (this->memory[addr] >> 1) | this->C;
            this->memory[addr] = res;
            break;
        } case 0x7E: {
            break; 
        } case 0x6A: { // register A rotate left
            this->C = (bool)(this->A & 0x01);
            res = (this->A >> 1) | this->C;
            this->A = res;
            break; 
        } case 0x66: { 
            break; 
        } case 0x76: {
            break; 
        } default: {
            std::cout << "invalid opcode for ROR" << std::endl;
        }
    }
    this->Z = res == 0;
    this->N = (bool)(res & 0x80);
}

void W65C02S::RTS(uint8_t opcode) {
    switch (opcode) {
        case 0x60: {
            // This assumes that the return address is the topmost 16-bits on the stack currently. If someone's left some stuff there, it'll fail.
            uint16_t returnAddr = this->memory[0x0100 + (++this->S)];
            returnAddr += this->memory[0x0100 + (++this->S)] << 8;
            // The 6502 reportedly did returnAddr + 1 before ending. Here, there's no need to do that since the main loop will increment PC before decoding the next instruction.
            this->PC = returnAddr;
            break;
        } default: {
            std::cout << "invalid opcode for RTS" << std::endl;
        }
    }
}

void W65C02S::SBC(uint8_t opcode) {
    // Values needed to calculate the flags
    // res = M - N - (~carry)
    uint8_t M = this->A;
    uint8_t N;
    uint16_t res;
    switch (opcode) {  // add absolute address with carry
        case 0xED: {
            uint16_t addr = this->memory[++this->PC];
            addr += this->memory[++this->PC] << 8;
            N = this->memory[addr];
            break;
        } case 0xFD: {
            break; 
        } case 0xF9: {
            break; 
        } case 0xE9: { // add immediate value with carry
            N = this->memory[++this->PC];
            break;
        } case 0xE5: {
            break; 
        } case 0xE1: {
            break; 
        } case 0xF5: {
            break; 
        } case 0xF2: {
            break; 
        } case 0xF1: {
            break; 
        } default: {
            std::cout << "invalid opcode for SBC" << std::endl;
        }
    }
    // calculate intermediate values
    res = M - N - (!this->C);
    // set flags
    this->C = res <= 255;
    this->Z = res == 0;
    this->V = (bool)((M ^ res) & (N ^ res) & 0x80); 
    this->N = (bool)(res & 0x80);
    // set accumulator value
    this->A = res;
}

void W65C02S::SEC(uint8_t opcode) {
    switch (opcode) {
        case 0x38: {
            this->C = true;
            break;
        } default: {
            std::cout << "invalid opcode for SEC" << std::endl;
        }
    }
}

void W65C02S::SED(uint8_t opcode) {
    switch (opcode) {
        case 0xF8: {
            this->D = true;
            break;
        } default: {
            std::cout << "invalid opcode for SED" << std::endl;
        }
    }
}

void W65C02S::SEI(uint8_t opcode) {
    switch (opcode) {
        case 0x78: {
            this->I = true;
            break;
        } default: {
            std::cout << "invalid opcode for SEI" << std::endl;
        }
    }
}

void W65C02S::STA(uint8_t opcode) {
    switch (opcode) {
        case 0x8D: {   // store at absolute address
            uint16_t addr = this->memory[++this->PC];
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
        } case 0x95: { // zero page indirect address with register X
            uint16_t addr = this->X + this->memory[++this->PC];
            this->memory[addr] = this->A;
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
            uint16_t addr = this->memory[++this->PC];
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
            uint16_t addr = this->memory[++this->PC];
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

void W65C02S::TXA(uint8_t opcode) {
    switch (opcode) {
        case 0x8A: {
            this->A = this->X;
            break;
        } default: {
            std::cout << "invalid opcode for TAX" << std::endl;
        }
    }
    this->Z = this->A == 0;
    this->N = (bool)(this->A & 0x80);
}

void W65C02S::TYA(uint8_t opcode) {
    switch (opcode) {
        case 0x98: {
            this->A = this->Y;
            break;
        } default: {
            std::cout << "invalid opcode for TAY" << std::endl;
        }
    }
    this->Z = this->A == 0;
    this->N = (bool)(this->A & 0x80);
}
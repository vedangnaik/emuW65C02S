#include "W65C02S.h"

W65C02S::W65C02S(int clockSpeedinHz, uint8_t* RAMPtr) {
    this->timeToWait = std::chrono::microseconds(1000000 / clockSpeedinHz);
    this->RAMPtr = RAMPtr;
    this->reset();
}

void W65C02S::reset() {
    this->A = this->X = this->Y = this->P = this->S = 0;
    this->PC = (this->RAMPtr[0xFFFD] << 8) + this->RAMPtr[0xFFFC];
}

// This cycle runs until the PC gets out of range. Technically this cannot happen on the actual microprocessor since the PC is only 16 bits long so can never reach 65536. One could put a JMP 0000 at the end to prevent this, but I'll make a proper strategy
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

        // sleep thread to simulate actual clock speed
        std::this_thread::sleep_for(this->timeToWait);

        // increment PC
        this->PC++;
    }
}

void W65C02S::printRegisters() {
    std::cout << "A: " << int(this->A) << std::endl;
    // TODO, add rest
}
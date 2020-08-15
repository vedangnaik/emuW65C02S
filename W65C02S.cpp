#include "W65C02S.h"

W65C02S::W65C02S(uint8_t* memory) {
    this->memory = memory;
    this->reset();
}

void W65C02S::reset() {
    this->A = this->X = this->Y = this->P = 0;
    this->S = 0xFF;
    this->PC = (this->memory[0xFFFD] << 8) + this->memory[0xFFFC];
}

void W65C02S::run() {
    // this->runnerThread = std::thread([this]() {
        
    // });
    while (1) {
        // fetch instruction
        this->IR = this->memory[this->PC];
        // decode instruction
        // TODO: error checking here in case of invalid instruction
        void (W65C02S::*executor)(uint8_t);
        executor = this->decoder[this->IR];
        // execute instruction
        (this->*(executor))(this->IR);
        // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        // increment PC
        this->PC++;
    }
}
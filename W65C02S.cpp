#include "W65C02S.h"

W65C02S::W65C02S(int clockSpeedinHz, uint8_t* RAMPtr) {
    this->timeToWait = std::chrono::microseconds(1000000 / clockSpeedinHz);
    this->memory = RAMPtr;
    this->reset();
}

void W65C02S::reset() {
    this->A = this->X = this->Y = this->P = 0;
    this->S = 0xFF;
    this->PC = (this->memory[0xFFFD] << 8) + this->memory[0xFFFC];
}

void W65C02S::run() {
    while (1) {
        // fetch instruction
        this->IR = this->memory[this->PC];
        // decode instruction
        // TODO: error checking here in case of invalid instruction
        void (W65C02S::*executor)(uint8_t);
        executor = this->decoder[this->IR];
        // execute instruction
        (this->*(executor))(this->IR);
        // print out A register's value for now
        std::cout << "S: " << (int)this->S << std::endl;
        // increment PC
        this->PC++;
    }
}
#include "W65C02S.h"

W65C02S::W65C02S(uint8_t* memory) {
    this->memory = memory;
    this->reset();
}

void W65C02S::reset() {
    this->A = this->X = this->Y = 0;
    this->S = 0xFF;
    this->PC = (this->memory[0xFFFD] << 8) + this->memory[0xFFFC];

    this->C = false; // no carry bit initially
    this->Z = true;  // the accumulator is zero to start
    this->I = false; // no interrupt handler is in progress at startup
    this->D = false; // decimal mode is off by default at startup
    this->B = false; // no software interrupt handler is in progress at startup
    this->V = false; // no overflow in the beginning
    this->N = false; // accumulator is not negative at startup
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
        // delay for dev work
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // increment PC
        this->PC++;
    }
}

uint8_t W65C02S::makePfromFlags() {
    return this->C        | 
        (this->Z    << 1) |
        (this->I    << 2) |
        (this->D    << 3) |
        (this->B    << 4) |
        (0x01       << 5) |
        (this->V    << 6) |
        (this->N    << 7);
}

void W65C02S::setFlagsFromP(uint8_t P) {
    this->C = P & 0b00000001;
    this->Z = P & 0b00000010;
    this->I = P & 0b00000100;
    this->D = P & 0b00001000;
    this->B = P & 0b00010000;
    this->V = P & 0b01000000;
    this->N = P & 0b10000000;
}
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
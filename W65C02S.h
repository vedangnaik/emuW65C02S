#ifndef W65C02S_H
#define W65C02S_H

#include <iostream>
#include <array>
#include <chrono>
#include <thread>

class W65C02S {
public:
    W65C02S(int clockSpeedinHz, uint8_t* RAMPtr);
    void reset();
    void startFetchDecodeExecute();

private:
    uint8_t* RAMPtr;
    std::chrono::microseconds timeToWait;

    // internal registers
    uint8_t IR;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint8_t S;
    uint16_t PC;
};

#endif
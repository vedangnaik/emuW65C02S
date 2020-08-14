#ifndef W65C02S_H
#define W65C02S_H

#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#define MAX_MEMSIZE 65536


class W65C02S {
public:
    W65C02S(int clockSpeedinHz, uint8_t* RAMPtr);
    void reset();
    void run();

private:
    uint8_t* memory;
    std::chrono::microseconds timeToWait;

    // internal registers
    uint8_t IR;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint8_t S;
    uint16_t PC;

    // opcodes to instruction hashmap - defined in decoder.cpp
    static std::map<uint8_t, void (W65C02S::*)(uint8_t)> decoder;

    // instruction executors - all defined in executors.cpp
    void NOP(uint8_t opcode);
    void LDA(uint8_t opcode);
    void STA(uint8_t opcode);
};

#endif
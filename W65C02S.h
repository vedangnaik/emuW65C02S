#ifndef W65C02S_H
#define W65C02S_H

#include <iostream>
#include <map>
#include <chrono>
#include <thread>

// Keep this above 0xFFFC lmao, otherwise the reset vector initialization segfaults
#define MAX_MEMSIZE 65536 


class W65C02S {
public:
    W65C02S(uint8_t* memory);
    void reset();
    void run();

    // internal registers
    uint8_t IR;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint8_t S;
    uint16_t PC;

private:
    uint8_t* memory;

    // opcodes to instruction hashmap - defined in decoder.cpp
    static std::map<uint8_t, void (W65C02S::*)(uint8_t)> decoder;

    // instruction executors - all defined in executors.cpp
    void ADC(uint8_t opcode);
    void DEC(uint8_t opcode);
    void DEX(uint8_t opcode);
    void DEY(uint8_t opcode);
    void INC(uint8_t opcode);
    void INX(uint8_t opcode);
    void INY(uint8_t opcode);
    void JMP(uint8_t opcode);
    void LDA(uint8_t opcode);
    void LDX(uint8_t opcode);
    void LDY(uint8_t opcode);
    void NOP(uint8_t opcode);
    void PHA(uint8_t opcode);
    void PHP(uint8_t opcode);
    void PHX(uint8_t opcode);
    void PHY(uint8_t opcode);
    void PLA(uint8_t opcode);
    void PLP(uint8_t opcode);
    void PLX(uint8_t opcode);
    void PLY(uint8_t opcode);
    void ROL(uint8_t opcode);
    void ROR(uint8_t opcode);
    void STA(uint8_t opcode);
    void STX(uint8_t opcode);
    void STY(uint8_t opcode);
};

#endif
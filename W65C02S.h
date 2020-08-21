#ifndef W65C02S_H
#define W65C02S_H

#include <iostream>
#include <sstream>
#include <map>
#include <chrono>
#include <thread>

// Keep this above 0xFFFC lmao, otherwise the reset vector initialization segfaults
#define MAX_MEMSIZE 65536
extern std::stringstream scrout;


class W65C02S {
public:
    W65C02S(uint8_t* memory);
    void reset();
    void run();

    uint8_t IR; // Instruction register
    uint8_t A; // Accumulator
    uint8_t X; // X register
    uint8_t Y; // Y register
    uint8_t S; // Stack pointer register
    uint16_t PC; // Program coutner
    // uint8_t P; // Processor status register. This register is not actually delclared since it's rebuiilt from the flags below anytime it's needed. This line it just for completion's sake 

    // The W65C02S's flags. The flags are stored individually instead of in the format speficied by the processor status register since otherwise a lot of bitmasking and whatnot would be required for operations which need individual flags.
    bool C; // Carry flag
    bool Z; // Zero flag
    bool I; // Interrupt request disable
    bool D; // Decimal mode
    bool B; // BRK command
    bool V; // Overflow flag
    bool N; // Negative flag

    // functions to create the processor status register's value from the flags and vice versa.
    uint8_t makePfromFlags();
    void setFlagsFromP(uint8_t P);

private:
    uint8_t* memory;

    // opcodes to instruction hashmap - defined in decoder.cpp
    static std::map<uint8_t, void (W65C02S::*)(uint8_t)> decoder;

    // instruction executors - all defined in executors.cpp
    void ADC(uint8_t opcode);
    void AND(uint8_t opcode);
    void BCC(uint8_t opcode);
    void BCS(uint8_t opcode);
    void BEQ(uint8_t opcode);
    void CLC(uint8_t opcode);
    void CLD(uint8_t opcode);
    void CLI(uint8_t opcode);
    void CLV(uint8_t opcode);
    void CMP(uint8_t opcode);
    void CPX(uint8_t opcode);
    void CPY(uint8_t opcode);
    void DEC(uint8_t opcode);
    void DEX(uint8_t opcode);
    void DEY(uint8_t opcode);
    void INC(uint8_t opcode);
    void INX(uint8_t opcode);
    void INY(uint8_t opcode);
    void JMP(uint8_t opcode);
    void JSR(uint8_t opcode);
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
    void RTS(uint8_t opcode);
    void SBC(uint8_t opcode);
    void SEC(uint8_t opcode);
    void SED(uint8_t opcode);
    void SEI(uint8_t opcode);
    void STA(uint8_t opcode);
    void STX(uint8_t opcode);
    void STY(uint8_t opcode);
    void TXA(uint8_t opcode);
    void TYA(uint8_t opcode);
};

#endif
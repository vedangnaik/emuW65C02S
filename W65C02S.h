#include <iostream>
#include <array>

class W65C02S {
public:
    W65C02S(uint8_t* RAMPtr);
    void reset();
    void run();

    // helper functions
    void printRegisters();

    // external pins of interest

private:
    uint8_t* RAMPtr;

    // internal registers
    uint8_t IR;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint8_t S;
    uint32_t PC;
    
    enum OPCODES {
        NOP = 0xea
    };
};
#include <iostream>
#include <array>
#include <chrono>
#include <thread>

class W65C02S {
public:
    W65C02S(int clockSpeedinHz, uint8_t* RAMPtr);
    void reset();
    void run();

    // helper functions
    void printRegisters();

    // external pins of interest

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
    uint32_t PC;
    
    enum OPCODES {
        NOP = 0xea
    };
};
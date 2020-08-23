#include "W65C02S.h"

std::map<uint8_t, void (W65C02S::*)(uint8_t)> W65C02S::decoder = {
    // ADC: Add accumulator to memory with carry
    {0x6D, &W65C02S::ADC},
    {0x7D, &W65C02S::ADC},
    {0x79, &W65C02S::ADC},
    {0x69, &W65C02S::ADC},
    {0x65, &W65C02S::ADC},
    {0x61, &W65C02S::ADC},
    {0x75, &W65C02S::ADC},
    {0x72, &W65C02S::ADC},
    {0x71, &W65C02S::ADC},
    // AND: Bitwise AND memory with accumulator
    {0x2D, &W65C02S::AND},
    {0x3D, &W65C02S::AND},
    {0x39, &W65C02S::AND},
    {0x29, &W65C02S::AND},
    {0x25, &W65C02S::AND},
    {0x21, &W65C02S::AND},
    {0x35, &W65C02S::AND},
    {0x32, &W65C02S::AND},
    {0x31, &W65C02S::AND},
    // ASL: Arithmetic shift one bit left
    {0x0E, &W65C02S::ASL},
    {0x1E, &W65C02S::ASL},
    {0x0A, &W65C02S::ASL},
    {0x06, &W65C02S::ASL},
    {0x16, &W65C02S::ASL},
    // BCC: Branch if carry = 0
    {0x90, &W65C02S::BCC},
    // BCS: Branch if carry = 1
    {0xB0, &W65C02S::BCS},
    // BEQ: Branch if zero = 1
    {0xF0, &W65C02S::BEQ},
    // BIT: Bit test (see https://retrocomputing.stackexchange.com/questions/11108/why-does-the-6502-have-the-bit-instruction for uses)
    {0x2C, &W65C02S::BIT},
    {0x3C, &W65C02S::BIT},
    {0x89, &W65C02S::BIT},
    {0x24, &W65C02S::BIT},
    {0x34, &W65C02S::BIT},
    // BMI: Branch if result is negative
    {0x30, &W65C02S::BMI},
    // BNE: Branch if not equal
    {0xD0, &W65C02S::BNE},
    // BPL: Branch if result is positive
    {0x10, &W65C02S::BPL},
    // BVC: Branch on overflow clear
    {0x50, &W65C02S::BVC},
    // BVS: Branch on overflow set
    {0x70, &W65C02S::BVS},
    // CLC: Clear carry flag
    {0x18, &W65C02S::CLC},
    // CLD: Clear decimal mode flag
    {0x18, &W65C02S::CLD},
    // CLI: Clear interrupt disable flag
    {0x18, &W65C02S::CLI},
    // CLV: Clear overflow flag
    {0x18, &W65C02S::CLV},
    // CMP: Compare memory with accumulator
    {0xCD, &W65C02S::CMP},
    {0xDD, &W65C02S::CMP},
    {0xD9, &W65C02S::CMP},
    {0xC9, &W65C02S::CMP},
    {0xC5, &W65C02S::CMP},
    {0xC1, &W65C02S::CMP},
    {0xD5, &W65C02S::CMP},
    {0xD2, &W65C02S::CMP},
    {0xD1, &W65C02S::CMP},
    // CPX: Compare memory with register X
    {0xEC, &W65C02S::CPX},
    {0xE0, &W65C02S::CPX},
    {0xE4, &W65C02S::CPX},
    // CPY: Compare memory with register Y
    {0xCC, &W65C02S::CPY},
    {0xC0, &W65C02S::CPY},
    {0xC4, &W65C02S::CPY},
    // DEC: Decrement memory or register A
    {0xCE, &W65C02S::DEC},
    {0xDE, &W65C02S::DEC},
    {0x3A, &W65C02S::DEC},
    {0xC6, &W65C02S::DEC},
    {0xD6, &W65C02S::DEC},
    // DEX: Decrement register X
    {0xCA, &W65C02S::DEX},
    // DEY: Decrement register Y
    {0x88, &W65C02S::DEY},
    // INC: Increment memory or register A
    {0xEE, &W65C02S::INC},
    {0xFE, &W65C02S::INC},
    {0x1A, &W65C02S::INC},
    {0xE6, &W65C02S::INC},
    {0xF6, &W65C02S::INC},
    // INX: Increment register X
    {0xE8, &W65C02S::INX},
    // INY: Increment register Y
    {0xC8, &W65C02S::INY},
    // JMP: Jump unconditionally
    {0x4C, &W65C02S::JMP},
    // JSR: Jump to subroutine
    {0x20, &W65C02S::JSR},
    // LDA: Load register A
    {0xAD, &W65C02S::LDA},
    {0xBD, &W65C02S::LDA},
    {0xA9, &W65C02S::LDA},
    {0xB9, &W65C02S::LDA},
    {0xA5, &W65C02S::LDA},
    {0xA1, &W65C02S::LDA},
    {0xB5, &W65C02S::LDA},
    {0xB2, &W65C02S::LDA},
    {0xB1, &W65C02S::LDA},
    // LDX: Load register X
    {0xAE, &W65C02S::LDX},
    {0xBE, &W65C02S::LDX},
    {0xA2, &W65C02S::LDX},
    {0xA6, &W65C02S::LDX},
    {0xB6, &W65C02S::LDX},
    // LDY: Load register Y
    {0xAC, &W65C02S::LDY},
    {0xBC, &W65C02S::LDY},
    {0xA0, &W65C02S::LDY},
    {0xA4, &W65C02S::LDY},
    {0xB4, &W65C02S::LDY},
    // NOP: No operation
    {0xEA, &W65C02S::NOP},
    // PH_: Push register onto stack
    {0x48, &W65C02S::PHA},
    {0x08, &W65C02S::PHP},
    {0xDA, &W65C02S::PHX},
    {0x5A, &W65C02S::PHY},
    // PL_: Pull from stack into register
    {0x68, &W65C02S::PLA},
    {0x28, &W65C02S::PLP},
    {0xFA, &W65C02S::PLX},
    {0x7A, &W65C02S::PLY},
    // ROL: Bitwise rotate left
    {0x2E, &W65C02S::ROL},
    {0x3E, &W65C02S::ROL},
    {0x2A, &W65C02S::ROL},
    {0x26, &W65C02S::ROL},
    {0x36, &W65C02S::ROL},
    // ROR: Bitwise rotate right
    {0x6E, &W65C02S::ROR},
    {0x7E, &W65C02S::ROR},
    {0x6A, &W65C02S::ROR},
    {0x66, &W65C02S::ROR},
    {0x76, &W65C02S::ROR},
    // RTS: Return from subroutine
    {0x60, &W65C02S::RTS},
    // SBC: Subtract with carry
    {0xED, &W65C02S::SBC},
    {0xFD, &W65C02S::SBC},
    {0xF9, &W65C02S::SBC},
    {0xE9, &W65C02S::SBC},
    {0xE5, &W65C02S::SBC},
    {0xE1, &W65C02S::SBC},
    {0xF5, &W65C02S::SBC},
    {0xF2, &W65C02S::SBC},
    {0xF1, &W65C02S::SBC},
    // SEC: Set carry flag
    {0x38, &W65C02S::SEC},
    // SED: Set decimal mode flag
    {0xF8, &W65C02S::SED},
    // SEI: Set interrupt disable flag
    {0x78, &W65C02S::SEI},
    // STA: Store register A
    {0x8D, &W65C02S::STA},
    {0x9D, &W65C02S::STA},
    {0x99, &W65C02S::STA},
    {0x85, &W65C02S::STA},
    {0x81, &W65C02S::STA},
    {0x95, &W65C02S::STA},
    {0x92, &W65C02S::STA},
    {0x91, &W65C02S::STA},
    // STX: Store register X
    {0x8E, &W65C02S::STX},
    {0x86, &W65C02S::STX},
    {0x96, &W65C02S::STX},
    // STY: Store register Y
    {0x8C, &W65C02S::STY},
    {0x84, &W65C02S::STY},
    {0x94, &W65C02S::STY},
    // TXA: Transfer X register to accumulator
    {0x8A, &W65C02S::TXA},
    // TYA: Transfer Y register to accumulator
    {0x98, &W65C02S::TYA}
};
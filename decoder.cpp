#include "W65C02S.h"

std::map<uint8_t, void (W65C02S::*)(uint8_t)> W65C02S::decoder = {
    // JMP: Jump unconditionally
    {0x4C, &W65C02S::JMP},
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
    // STA: Store register A
    {0x8D, &W65C02S::STA},
    // STX: Store register X
    {0x8E, &W65C02S::STX},
    {0x86, &W65C02S::STX},
    {0x96, &W65C02S::STX},
    // STY: Store register Y
    {0x8C, &W65C02S::STY},
    {0x84, &W65C02S::STY},
    {0x94, &W65C02S::STY},
};
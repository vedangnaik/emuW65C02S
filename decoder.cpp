#include "W65C02S.h"

std::map<uint8_t, void (W65C02S::*)(uint8_t)> W65C02S::decoder = {
    {0xea, &W65C02S::NOP},
    {0xa9, &W65C02S::LDA},
    {0x8d, &W65C02S::STA},
    {0x4c, &W65C02S::JMP}
};
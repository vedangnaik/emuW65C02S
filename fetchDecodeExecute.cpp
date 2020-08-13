#include "W65C02S.h"
#include "W65C02SOpcodes.h"

// This cycle runs forever. If the PC is allowed to go past 65535, it will wrap back around to 0 and continue, which will probably break everything amazingly. Note that one loop does not necessarily equal one clock cycle: depending on the instruction, the cases in the switch-case will execute the required loops internally.
void W65C02S::startFetchDecodeExecute() {
    while (1) {
        // fetch instruction
        this->IR = this->RAMPtr[this->PC];

        // decode instruction
        std::cout << this->PC << "  " << std::hex << (int)this->IR << std::endl;
        switch (this->IR) {
            case OPCODES::NOP:
                break;
            case OPCODES::LOAD_A_IMMEDIATE: {
                this->PC++;
                this->A = this->RAMPtr[this->PC];
                break;
            } case OPCODES::STORE_A_ABSOLUTE: {
                int addr = this->RAMPtr[++this->PC];
                addr += this->RAMPtr[++this->PC] << 8;
                this->RAMPtr[addr] = this->A;
                break;
            } default: {
                break;
            }
        }

        // sleep thread to simulate actual clock speed
        std::this_thread::sleep_for(this->timeToWait);

        // increment PC
        this->PC++;
    }
}
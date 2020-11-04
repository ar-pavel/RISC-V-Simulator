#include "rv32i.h"

//Save the m argument in the mem member variable for use later when disassembling
rv32i::rv32i(memory *m)
{
    this->mem = m;
}

// This method will be used to disassemble the instructions in the simulated memory
// To perform this task, set pc to zero and then, for each 32-bit word in the memory
void rv32i::disasm(void)
{
}

// This function must be capable of handling any possible insn value.
// It is the purpose of this function to return a std::string containing
// the disassembled instruction text. This function will not print anything.
std::string rv32i::decode(uint32_t insn) const
{
}
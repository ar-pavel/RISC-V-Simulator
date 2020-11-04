#include <iostream>

#include "rv32i.h"
#include "hex.h"

//Save the m argument in the mem member variable for use later when disassembling
rv32i::rv32i(memory *m)
{
    this->mem = m;
}

// This method will be used to disassemble the instructions in the simulated memory
// To perform this task, set pc to zero and then, for each 32-bit word in the memory
void rv32i::disasm(void)
{
    // set pc to zero
    for (this->pc = 0;; this->pc < this->mem->get_size(); this->pc += 4)
    {
        // print the 32-bit hex address in the pc register
        std::cout << hex32(this->pc) << " ";

        // fetch the 32-bit instruction from memory at the address in the pc register
        uint32_t insn = this->mem->get32(this->pc);

        // print the instruction as a 32-bit hex value
        std::cout << hex32(insn) << " ";

        // pass the fetched instruction to decode() to decode and render the instruction into a
        // printable std::string
        std::string decoded_insn = decode(insn);

        // print the decoded instruction string returned from decode()

        // increment pc by 4 (point to the next instruction)
    }
}

// This function must be capable of handling any possible insn value.
// It is the purpose of this function to return a std::string containing
// the disassembled instruction text. This function will not print anything.
std::string rv32i::decode(uint32_t insn) const
{
}
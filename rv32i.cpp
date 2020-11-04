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
    for (this->pc = 0; this->pc < this->mem->get_size(); this->pc += 4)
    {
        // print the 32-bit hex address in the pc register
        std::cout << hex32(this->pc) << ": ";

        // fetch the 32-bit instruction from memory at the address in the pc register
        uint32_t insn = this->mem->get32(this->pc);

        // print the instruction as a 32-bit hex value
        std::cout << hex32(insn) << " ";

        // pass the fetched instruction to decode() to decode and render the instruction into a
        // printable std::string
        std::string decoded_insn = decode(insn);

        // print the decoded instruction string returned from decode()
        std::cout << decoded_insn << " ";
        // increment pc by 4 (point to the next instruction)
    }
}

// This function must be capable of handling any possible insn value.
// It is the purpose of this function to return a std::string containing
// the disassembled instruction text. This function will not print anything.
std::string rv32i::decode(uint32_t insn) const
{
    return "";
}

// Extract and return the opcode field from the given instruction
uint32_t rv32i::get_opcode(uint32_t insn)
{
    // 0x7f = 127
    return (insn & 0x0000007f);
}

// Extract and return the rd field from the given instruction
uint32_t rv32i::get_rd(uint32_t insn)
{
    return (insn & 0xF80);
}

// Extract and return the funct3 field from the given instruction
uint32_t rv32i::get_funct3(uint32_t insn)
{
    return (insn & 0x7000);
}

// Extract and return the rs1 field from the given instruction
uint32_t rv32i::get_rs1(uint32_t insn)
{
    return (insn & 0xF8000);
}

// Extract and return the rs2 field from the given instruction
uint32_t get_rs2(uint32_t insn)
{
    return (insn & 0x1F00000);
}

// Extract and return the funct7 field from the given instruction
uint32_t rv32i::get_funct7(uint32_t insn)
{
    return (insn & 0xFE000000);
}

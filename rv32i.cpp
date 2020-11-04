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
    uint32_t opcode = get_opcode(insn);

    switch (opcode)
    {
    case opcode_lui:
        return render_lui(insn);

    case opcode_auipc:
        return render_auipc(insn);

    case opcode_jal:
        return render_jal(insn);

    case opcode_jalr:
        return render_jalr(insn);

        // case opcode_btype:
        //     return render_btype(insn, mnemonic);

    case opcode_load_imm:
        /* code */
        break;

    default:
        return render_illegal_insn();
    }

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

// Extract and return the imm_i field from the given instruction
int32_t rv32i::get_imm_i(uint32_t insn)
{
    return (insn & 0xFFFF00000);
}

// Extract and return the imm_u field from the given instruction static int32_t get_imm_u(uint32_t insn);
int32_t rv32i::get_imm_u(uint32_t insn)
{
    return (insn & 0xFFFFF0000);
}

// Extract and return the imm_b field from the given instruction
// bits [12][10:5][4:1][11]
int32_t rv32i::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0xF00) >> 7; // extract & shift bits 8 - 11 to 1 - 4
    imm_b |= (insn & 0x7E000000) >> 20;  // extract & shift bits 25 - 30 to 5 - 10
    imm_b |= (insn & 0x80) << 4;         // extract & shift bit 7 to 11
    imm_b |= (insn & 0x80000000) >> 19;  // extract & shift bit 31 to 12
    if (insn & 0x80000000)               // sign - extend
        imm_b |= 0xFFFFE000;
    return imm_b;
}

// Extract and return the imm_s field from the given instruction
// bits [11:5][4:0]
int32_t rv32i ::get_imm_s(uint32_t insn)
{
    int32_t imm_s = (insn & 0xfe000000) >> 20; // extract & shift bits 25 - 31 to 5 - 11
    imm_s |= (insn & 0x00000f80) >> 7;         // extract & shift bits 7 - 11 to 0 - 4
    if (insn & 0x80000000)                     // sign - extend
        imm_s |= 0xFFFFF000;

    return imm_s;
}

// Extract and return the imm_j field from the given instruction
// bits [20][10:1][11][19:12]
int32_t rv32i::get_imm_j(uint32_t insn)
{
    int32_t imm_j = (insn & 0x7FE00000) >> 20; // extract & shift bits 21 - 30 to 1 - 10
    imm_j |= (insn & 0x100000) >> 9;           // extract & shift bit 20 to bit 11
    imm_j |= (insn & 0xFF000);                 // extract & shift bits 12 - 19 to 12 - 19
    imm_j |= (insn & 0x80000000) >> 11;        // extract & shift bit 31 to 20
    if (insn & 0x80000000)                     // sign - extend
        imm_j |= 0xFFE00000;
    return imm_j;
}

void rv32i::exec_slt(uint32_t insn, std::ostream *pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;
    if (pos)
    {
        std::string s = render_rtype(insn, "slt     ");
        s.resize(instruction_width, ' ');
        // slt x4,x14,x15 // x4 = (0xf0f0f0f0 < 0xf0f0f0f0) ? 1 : 0 = 0x00000000
        *pos << s << "// "
             << "x4 = ("
             << hex0x32(rs1)
             << " < "
             << hex0x32(rs2)
             << " ? 1 : 0 = "
             << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

void rv32i::exec_ebreak(uint32_t insn, std::ostream *pos)
{
    if (pos)
    {
        std::string s = render_ebreak(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "//  HALT";
    }
    halt = true;
}

std::string rv32i::render_illegal_insn() const
{

    return "";
}

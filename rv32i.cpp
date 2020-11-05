#include <iostream>
#include <sstream>

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

    case opcode_btype:
        switch (get_funct3(insn))
        {
        case 0b000:
            return render_btype(insn, " beq    ");
        case 0b001:
            return render_btype(insn, " bne    ");

        case 0b100:
            return render_btype(insn, " blt    ");

        case 0b101:
            return render_btype(insn, " bge    ");

        case 0b110:
            return render_btype(insn, " bltu   ");

        case 0b111:
            return render_btype(insn, " bgeu   ");

        default:
            return render_eror(insn);
        }
        break;

    case opcode_load_imm:
        switch (get_funct3(insn))
        {
        case 0b000:
            return render_itype_load(insn, " lb     ");
        case 0b001:
            return render_itype_load(insn, " lh     ");
        case 0b010:
            return render_itype_load(insn, " lw     ");
        case 0b100:
            return render_itype_load(insn, " lbu    ");
        case 0b101:
            return render_itype_load(insn, " lhu    ");
        default:
            return render_eror(insn);
        }
        break;

    case opcode_alu_imm:
        switch (get_funct3(insn))
        {
        case 0b000:
            return render_itype_alu(insn, " addi   ", get_imm_i(insn));

        case 0b010:
            return render_itype_alu(insn, " slti   ", get_imm_i(insn));

        case 0b011:
            return render_itype_alu(insn, " sltiu  ", get_imm_i(insn));

        case 0b100:
            return render_itype_alu(insn, " xori   ", get_imm_i(insn));

        case 0b110:
            return render_itype_alu(insn, " ori    ", get_imm_i(insn));

        case 0b111:
            return render_itype_alu(insn, " andi   ", get_imm_i(insn));

        case 0b001:
            return render_itype_alu(insn, " slli   ", get_imm_i(insn));

        case 0b101:
            //checks get_funct7 value
            switch (get_funct7(insn))
            {
            case 0b0000000:
                return render_itype_alu(insn, " srli   ", get_imm_i(insn));

            case 0b0100000:
                return render_itype_alu(insn, " srai   ", get_imm_i(insn));

            default:
                return render_eror(insn);
            }
            break;
        default:
            return render_eror(insn);
        }

        break;

    case opcode_stype: // S - type store instructions
        //checks get_funct3 value
        switch (get_funct3(insn))
        {
        case 0b000:
            return render_stype(insn, " sb     ");

        case 0b001:
            return render_stype(insn, " sh     ");

        case 0b010:
            return render_stype(insn, " sw     ");

        default:
            return render_eror(insn);
        }

    case opcode_rtype: // R-type
        //checks get_funct3 value
        switch (get_funct3(insn))
        {
        case 0b000:
            //checks get_funct7 value
            switch (get_funct7(insn))
            {
            case 0b0000000:
                return render_rtype(insn, " add    ");

            case 0b0100000:
                return render_rtype(insn, " sub    ");

            default:
                return render_eror(insn);
            }
            break;
        case 0b001:
            return render_rtype(insn, " sll    ");

        case 0b010:
            return render_rtype(insn, " slt    ");

        case 0b011:
            return render_rtype(insn, " sltu   ");

        case 0b100:
            return render_rtype(insn, " xor    ");

        case 0b101:
            //checks get_funct7 value
            switch (get_funct7(insn))
            {
            case 0b0000000:
                return render_rtype(insn, " srl    ");

            case 0b0100000:
                return render_rtype(insn, " sra    ");

            default:
                return render_eror(insn);
            }
            break;
        case 0b110:
            return render_rtype(insn, " or     ");

        case 0b111:
            return render_rtype(insn, " and    ");
        }

    case opcode_fenc_opt: //fence operation
        return render_fence(insn);

    case opcode_exc:
        switch (get_funct7(insn) + get_rs2(insn))
        {
        case 0b000000000000:
            return render_ecall(insn);
        case 0b000000000001:
            return render_ebreak(insn);
        default:
            return render_eror(insn);
        }

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
    //extracts 6 - 0 bits
    return (insn & 0xF80) >> 7;
}

// Extract and return the funct3 field from the given instruction
uint32_t rv32i::get_funct3(uint32_t insn)
{
    return (insn & 0x7000) >> 12;
}

// Extract and return the rs1 field from the given instruction
uint32_t rv32i::get_rs1(uint32_t insn)
{
    return (insn & 0xF8000) >> 15;
}

// Extract and return the rs2 field from the given instruction
uint32_t get_rs2(uint32_t insn)
{
    return (insn & 0x1F00000) >> 20;
}

// Extract and return the funct7 field from the given instruction
uint32_t rv32i::get_funct7(uint32_t insn)
{
    return (insn & 0xFE000000) >> 25;
}

// Extract and return the imm_i field from the given instruction
int32_t rv32i::get_imm_i(uint32_t insn)
{
    int32_t imm_i = (insn & 0xFFFF00000) >> 20;
    if (insn & 0x80000000) // sign - extend
        imm_i |= 0xFFFFF000;
    return imm_i;
}

// Extract and return the imm_u field from the given instruction static int32_t get_imm_u(uint32_t insn)
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
        *pos << std::endl;
        *pos << "Execution terminated by EBREAK instruction";
    }
    halt = true;
}

std::string rv32i::render_illegal_insn() const
{
    std::ostringstream os;

    os << " ERRROR: UNIMPLEMENTED INSTRUCTION";

    return os.str();
}
std::string rv32i::render_lui(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value

    os << " lui     x" << std::dec << get_rd(insn) << ",0x" << std::hex << ((get_imm_u(insn) >> 12) & 0x0fffff);

    return os.str();
}
std::string rv32i::render_auipc(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << " auipc   x" << std::dec << get_rd(insn) << ",0x" << std::hex << ((get_imm_u(insn) >> 12) & 0x0fffff);

    return os.str();
}
std::string rv32i::render_jal(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << " jal     x" << std::dec << get_rd(insn) << ",0x" << std::hex << ((get_imm_j(insn) + pc));

    return os.str();
}
std::string rv32i::render_jalr(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << " jalr    x" << std::dec << get_rd(insn) << "," << get_imm_i(insn) << "(x" << get_rs1(insn) << ")";
    return os.str();
}

std::string rv32i::render_btype(uint32_t insn, const char *mnemonic) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << mnemonic;
    os << " x" << std::dec << get_rs1(insn) << ",x" << get_rs2(insn) << ","
       << "0x" << std::hex << (get_imm_b(insn) + pc);
    return os.str();
}

std::string rv32i::render_itype_load(uint32_t insn, const char *mnemonic) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << mnemonic;
    os << " x" << std::dec << get_rd(insn) << "," << get_imm_i(insn) << "(x" << get_rs1(insn) << ")";
    return os.str();
}
std::string rv32i::render_stype(uint32_t insn, const char *mnemonic) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << mnemonic;
    os << " x" << std::dec << get_rs2(insn) << "," << get_imm_s(insn) << "(x" << get_rs1(insn) << ")";

    return os.str();
}

std::string rv32i::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value

    os << mnemonic;

    if (get_funct3(insn) == 0b101 || get_funct3(insn) == 0b001)
    {
        os << " x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << "," << ((insn & 0x1f00000) >> 20);
    }
    else
    {
        os << " x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << "," << get_imm_i(insn);
    }

    return os.str();
}

std::string rv32i::render_rtype(uint32_t insn, const char *mnemonic) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << mnemonic;
    os << " x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << ",x" << get_rs2(insn);
    return os.str();
}

std::string rv32i::render_fence(uint32_t insn) const
{
    std::ostringstream os;
    std::string pred, succ;

    os << hex32(insn) << " "; // the instruction hex value

    //checks if 1 bit of pred is on
    if ((insn & (1 << 27)) != 0)
    {
        pred += 'i';
    }
    //checks if 2 bit of pred is on
    if ((insn & (1 << 26)) != 0)
    {
        pred += 'o';
    }
    //checks if 3 bit of pred is on
    if ((insn & (1 << 25)) != 0)
    {
        pred += 'r';
    }
    //checks if 4 bit of pred is on
    if ((insn & (1 << 24)) != 0)
    {
        pred += 'w';
    }
    //checks if 1 bit of succesor is on
    if ((insn & (1 << 23)) != 0)
    {
        succ += 'i';
    }
    //checks if 2 bit of succesor is on
    if ((insn & (1 << 22)) != 0)
    {
        succ += 'o';
    }
    //checks if 3 bit of succesor is on
    if ((insn & (1 << 21)) != 0)
    {
        succ += 'r';
    }
    //checks if 4 bit of succesor is on
    if ((insn & (1 << 20)) != 0)
    {
        succ += 'w';
    }
    os << " fence   " << pred << "," << succ;
    //std::hex << ((insn & 0xf000000) >> 24)

    return os.str();
}

std::string rv32i::render_ecall(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << " ecall";

    return os.str();
}

std::string rv32i::render_ebreak(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << " ebreak";
    return os.str();
}

std::string rv32i::render_eror(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << " ERROR ";
    return os.str();
}

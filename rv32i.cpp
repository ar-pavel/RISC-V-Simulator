#include <iostream>
#include <sstream>

#include "hex.h"
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
    // set pc to zero
    for (this->pc = 0; this->pc < this->mem->get_size(); this->pc += 4)
    {
        // print the 32-bit hex address in the pc register
        std::cout << hex32(this->pc) << ": ";

        // fetch the 32-bit instruction from memory at the address in the pc register
        uint32_t insn = this->mem->get32(this->pc);

        // print the instruction as a 32-bit hex value
        // std::cout << hex32(insn) << " ";

        // pass the fetched instruction to decode() to decode and render the instruction into a
        // printable std::string
        std::string decoded_insn = decode(insn);

        // print the decoded instruction string returned from decode()
        std::cout << decoded_insn << "\n";
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
uint32_t rv32i::get_rs2(uint32_t insn)
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
    if (insn & 0x80000000) // sign extend
        imm_i |= 0xFFFFF000;
    return imm_i;
}

// Extract and return the imm_u field from the given instruction static int32_t get_imm_u(uint32_t insn)
int32_t rv32i::get_imm_u(uint32_t insn)
{
    return (insn & 0xFFFFF000);
}

// Extract and return the imm_b field from the given instruction
// bits [12][10:5][4:1][11]
int32_t rv32i::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0xF00) >> 7; // extract & shift bits 8 - 11 to 1 - 4
    imm_b |= (insn & 0x7E000000) >> 20;  // extract & shift bits 25 - 30 to 5 - 10
    imm_b |= (insn & 0x80) << 4;         // extract & shift bit 7 to 11
    imm_b |= (insn & 0x80000000) >> 19;  // extract & shift bit 31 to 12
    if (insn & 0x80000000)               // sign extend
        imm_b |= 0xFFFFE000;
    return imm_b;
}

// Extract and return the imm_s field from the given instruction
// bits [11:5][4:0]
int32_t rv32i ::get_imm_s(uint32_t insn)
{
    int32_t imm_s = (insn & 0xfe000000) >> 20; // extract & shift bits 25 - 31 to 5 - 11
    imm_s |= (insn & 0x00000f80) >> 7;         // extract & shift bits 7 - 11 to 0 - 4
    if (insn & 0x80000000)                     // sign extend
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
    if (insn & 0x80000000)                     // sign extend
        imm_j |= 0xFFE00000;
    return imm_j;
}

// set instuction display flag
void rv32i::set_show_instructions(bool b)
{
    this->show_instructions = b;
}

// set register hart display flag
void rv32i::set_show_registers(bool b)
{
    this->show_registers = b;
}

// check if the program is halted
bool rv32i::is_halted() const
{
    return this->halt;
}

// reset RISC-V Simulation
void rv32i::reset()
{
    this->regs.reset();     // reset general purpose registers
    this->pc = 0;           // set program counter to zero
    this->insn_counter = 0; // set instruction counter to zero
    this->halt = false;     // setting 'halt' flag to false
}

// dump the simulator state
void rv32i::dump() const
{
    this->regs.dump();
    std::cout << " pc " << hex32(this->pc) << std::endl;
}

// function to execute an instruction
void rv32i::tick()
{
    this->insn_counter++; // increment instruction counter

    // show GP register hart
    if (this->show_registers)
        regs.dump();

    // fetch the 32-bit instruction from memory at the address in the pc register
    uint32_t insn = this->mem->get32(pc);

    // show instructions
    if (this->show_instructions)
    {
        // print the 32-bit hex address in the pc register
        std::cout << hex32(pc) << ": ";

        // print the instruction as a 32-bit hex value
        // std::cout << hex32(insn) << " ";

        // execute instruction and render instruction and simulation details
        dcex(insn, &std::cout);
    }
    else
    {
        // execute instruction without rendering instruction and simulation details
        dcex(insn, nullptr);
    }
}

// function to execute instructions loaded from file
void rv32i::run(uint64_t limit)
{
    // storing memory size to the x2 register
    this->regs.set(2, this->mem->get_size());

    // execute
    while (true)
    {
        if (limit && this->insn_counter >= limit) // if execution limit is reached
        {
            break;
        }
        if (is_halted()) // if the program is halted
        {
            return;
        }

        tick(); // execute one instruction at a time
    }
    std::cout << render_total_insn_exec(this->insn_counter) << std::endl;
}

/*****************************************
 * Execution Handler function
 * **************************************/

void rv32i::dcex(uint32_t insn, std::ostream *pos)
{

    uint32_t opcode = get_opcode(insn);

    switch (opcode)
    {
    case opcode_lui:
        exec_lui(insn, pos);
        break;
    case opcode_auipc:
        exec_auipc(insn, pos);
        break;
    case opcode_jal:
        exec_jal(insn, pos);
        break;
    case opcode_jalr:
        exec_jalr(insn, pos);
        break;
    case opcode_btype:
        switch (get_funct3(insn))
        {
        case 0b000:
            // exec_btype(insn, " beq    ", pos);
            exec_beq(insn, pos);
            break;
        case 0b001:
            // exec_btype(insn, " bne    ", pos);
            exec_bne(insn, pos);
            break;
        case 0b100:
            // exec_btype(insn, " blt    ", pos);
            exec_blt(insn, pos);
            break;
        case 0b101:
            // exec_btype(insn, " bge    ", pos);
            exec_bge(insn, pos);
            break;
        case 0b110:
            // exec_btype(insn, " bltu   ", pos);
            exec_bltu(insn, pos);
            break;
        case 0b111:
            // exec_btype(insn, " bgeu   ", pos);
            exec_bgeu(insn, pos);
            break;
        default:
            exec_eror(insn, pos);
            break;
        }
        break;

    case opcode_load_imm:
        switch (get_funct3(insn))
        {
        case 0b000:
            // exec_itype_load(insn, " lb     ", pos);
            exec_lb(insn, pos);
            break;
        case 0b001:
            // exec_itype_load(insn, " lh     ", pos);
            exec_lh(insn, pos);
            break;
        case 0b010:
            // exec_itype_load(insn, " lw     ", pos);
            exec_lw(insn, pos);
            break;
        case 0b100:
            // exec_itype_load(insn, " lbu    ", pos);
            exec_lbu(insn, pos);
            break;
        case 0b101:
            // exec_itype_load(insn, " lhu    ", pos);
            exec_lhu(insn, pos);
            break;
        default:
            exec_eror(insn, pos);
            break;
        }
        break;

    case opcode_alu_imm:
        switch (get_funct3(insn))
        {
        case 0b000:
            // exec_itype_alu(insn, " addi   ", get_imm_i(insn), pos);
            exec_addi(insn, pos);
            break;

        case 0b010:
            // exec_itype_alu(insn, " slti   ", get_imm_i(insn), pos);
            exec_slti(insn, pos);
            break;
        case 0b011:
            // exec_itype_alu(insn, " sltiu  ", get_imm_i(insn), pos);
            exec_sltiu(insn, pos);
            break;
        case 0b100:
            // exec_itype_alu(insn, " xori   ", get_imm_i(insn), pos);
            exec_xori(insn, pos);
            break;
        case 0b110:
            // exec_itype_alu(insn, " ori    ", get_imm_i(insn), pos);
            exec_ori(insn, pos);
            break;
        case 0b111:
            // exec_itype_alu(insn, " andi   ", get_imm_i(insn), pos);
            exec_andi(insn, pos);
            break;
        case 0b001:
            // exec_itype_alu(insn, " slli   ", get_imm_i(insn), pos);
            exec_slli(insn, pos);
            break;
        case 0b101:
            //checks get_funct7 value
            switch (get_funct7(insn))
            {
            case 0b0000000:
                // exec_itype_alu(insn, " srli   ", get_imm_i(insn), pos);
                exec_srli(insn, pos);
                break;
            case 0b0100000:
                // exec_itype_alu(insn, " srai   ", get_imm_i(insn), pos);
                exec_srai(insn, pos);
                break;
            default:
                exec_eror(insn, pos);
                break;
            }
            break;
        default:
            exec_eror(insn, pos);
            break;
        }

        break;

    case opcode_stype: // S - type store instructions
        //checks get_funct3 value
        switch (get_funct3(insn))
        {
        case 0b000:
            // exec_stype(insn, " sb     ", pos);
            exec_sb(insn, pos);
            break;
        case 0b001:
            // exec_stype(insn, " sh     ", pos);
            exec_sh(insn, pos);
            break;
        case 0b010:
            // exec_stype(insn, " sw     ", pos);
            exec_sw(insn, pos);
            break;
        default:
            exec_eror(insn, pos);
            break;
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
                // exec_rtype(insn, " add    ", pos);
                exec_add(insn, pos);
                break;
            case 0b0100000:
                // exec_rtype(insn, " sub    ", pos);
                exec_sub(insn, pos);
                break;
            default:
                exec_eror(insn, pos);
                break;
            }
            break;
        case 0b001:
            // exec_rtype(insn, " sll    ", pos);
            exec_sll(insn, pos);
            break;
        case 0b010:
            // exec_rtype(insn, " slt    ", pos);
            exec_slt(insn, pos);
            break;
        case 0b011:
            // exec_rtype(insn, " sltu   ", pos);
            exec_sltu(insn, pos);
            break;
        case 0b100:
            // exec_rtype(insn, " xor    ", pos);
            exec_xor(insn, pos);
            break;
        case 0b101:
            //checks get_funct7 value
            switch (get_funct7(insn))
            {
            case 0b0000000:
                // exec_rtype(insn, " srl    ", pos);
                exec_srl(insn, pos);
                break;
            case 0b0100000:
                // exec_rtype(insn, " sra    ", pos);
                exec_sra(insn, pos);
                break;
            default:
                exec_eror(insn, pos);
                break;
            }
            break;
        case 0b110:
            // exec_rtype(insn, " or     ", pos);
            exec_or(insn, pos);
            break;
        case 0b111:
            // exec_rtype(insn, " and    ", pos);
            exec_and(insn, pos);
            break;
        }

    case opcode_fenc_opt: //fence operation
        exec_fence(insn, pos);
        break;

    case opcode_exc:
        switch (get_funct7(insn) + get_rs2(insn))
        {
        case 0b000000000000:
            exec_ecall(insn, pos);
            break;
        case 0b000000000001:
            exec_ebreak(insn, pos);
            break;
        default:
            exec_eror(insn, pos);
            break;
        }

    default:
        exec_illegal_insn(insn, pos);
        break;
    }
}
//-------------------------------------------------------//

void rv32i::exec_illegal_insn(uint32_t insn, std::ostream *pos)
{
    this->halt = true;
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

/*****************************************
 * U-Type Instructions
 * **************************************/
void rv32i::exec_lui(uint32_t insn, std::ostream *pos)
{

    // store val to register reg
    uint32_t reg = get_rd(insn);
    int32_t val = get_imm_u(insn);
    this->regs.set(reg, val);
    // increment program counter
    this->pc = this->pc + 4;

    if (pos)
    {
        // 00000000: abcde237 lui x4,0xabcde // x4 = 0xabcde000
        std::string s = render_lui(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = " << hex0x32(val);
        *pos << std::endl;
    }
}
void rv32i::exec_auipc(uint32_t insn, std::ostream *pos)
{
    // store instruction addr + U-value to register reg
    uint32_t reg = get_rd(insn);
    int32_t val = get_imm_u(insn);
    this->regs.set(reg, val + this->pc);

    if (pos)
    {
        // 00000004: abcde217 auipc x4,0xabcde // x4 = 0x00000004 + 0xabcde000 = 0xabcde004
        std::string s = render_auipc(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = " << hex0x32(this->pc) << " + " << hex0x32(val) << " = " << hex0x32(val + this->pc);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}

/*****************************************
 * J-Type Instructions
 * **************************************/
void rv32i::exec_jal(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    uint32_t nxt_insn = this->pc + 4;
    this->regs.set(reg, nxt_insn);
    uint32_t imm_j = get_imm_j(insn);

    if (pos)
    {
        // 00000008: 008000ef jal x1,0x10 // x1 = 0x0000000c, pc = 0x00000008 + 0x00000008 = 0x00000010
        std::string s = render_jal(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = " << hex0x32(nxt_insn) << ", pc"
             << " = " << hex0x32(this->pc) << " + " << hex0x32(this->pc) << " = " << hex0x32(this->pc + this->pc);
        *pos << std::endl;
    }

    // jump to
    this->pc = this->pc + imm_j;
}

/*****************************************
 * R-Type Instructions
 * **************************************/
void rv32i::exec_add(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t sum = rs1 + rs2;
    this->regs.set(reg, sum);

    if (pos)
    {
        std::string s = render_rtype(insn, " add     ");
        s.resize(instruction_width, ' ');
        // 000000e0: 00f77233 and x4,x14,x15 // x4 = 0xf0f0f0f0 + 0xf0f0f0f0 = 0xf0f0f0f0
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " + "
             << hex0x32(rs2)
             << " = "
             << hex0x32(sum);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_and(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t res = (rs1 & rs2);
    this->regs.set(reg, res);
    if (pos)
    {
        std::string s = render_rtype(insn, " and     ");
        s.resize(instruction_width, ' ');
        // 000000dc: 00f76233 or x4,x14,x15 // x4 = 0xf0f0f0f0 | 0xf0f0f0f0 = 0xf0f0f0f0
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " | "
             << hex0x32(rs2)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_or(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t res = (rs1 | rs2);
    this->regs.set(reg, res);

    if (pos)
    {
        std::string s = render_rtype(insn, " or     ");
        s.resize(instruction_width, ' ');
        // 000000e0: 00f77233 and x4,x14,x15 // x4 = 0xf0f0f0f0 & 0xf0f0f0f0 = 0xf0f0f0f0
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " & "
             << hex0x32(rs2)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_sll(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    uint32_t rs2 = (uint32_t)this->regs.get(get_rs2(insn));
    uint32_t res = (rs1 << (rs2 & 0x1F));

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, res);

    if (pos)
    {
        std::string s = render_rtype(insn, "sll     ");
        s.resize(instruction_width, ' ');
        // 000000d0: 00f74233 xor x4,x14,x15 // x4 = 0xf0f0f0f0 ^ 0xf0f0f0f0 = 0x00000000
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " << "
             << hex0x32(rs2)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_slt(uint32_t insn, std::ostream *pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    uint32_t rs2 = (uint32_t)this->regs.get(get_rs2(insn));
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;
    if (pos)
    {
        std::string s = render_rtype(insn, "slt     ");
        s.resize(instruction_width, ' ');
        // slt x4,x14,x15 // x4 = (0xf0f0f0f0 < 0xf0f0f0f0) ? 1 : 0 = 0x00000000
        *pos << s << "// "
             << "x" << rd << " = ("
             << hex0x32(rs1)
             << " < "
             << hex0x32(rs2)
             << " ? 1 : 0 = "
             << hex0x32(val);
    }
    regs.set(rd, val);

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_sltu(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    uint32_t rs2 = (uint32_t)this->regs.get(get_rs2(insn));

    uint32_t reg = get_rd(insn);
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

    // this->regs.set(reg, (rs1 < rs2) ? 1 : 0);
    if (pos)
    {
        std::string s = render_rtype(insn, "sltu     ");
        s.resize(instruction_width, ' ');
        // 000000cc: 00f73233 sltu x4,x14,x15 // x4 = (0xf0f0f0f0 <U 0xf0f0f0f0) ? 1 : 0 = 0x00000000
        *pos << s << "// "
             << "x" << reg << " = ("
             << hex0x32(rs1)
             << " <U "
             << hex0x32(rs2)
             << " ? 1 : 0 = "
             << hex0x32(val);
    }
    regs.set(reg, val);

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_sra(uint32_t insn, std::ostream *pos)
{
    // signed data type for arithmetic shift
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));

    // arithmetic shift right
    int32_t res = (rs1 >> rs2);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, res);

    if (pos)
    {
        std::string s = render_rtype(insn, "sra     ");
        s.resize(instruction_width, ' ');
        // 000000d8: 40f751b3 sra x3,x14,x15 // x3 = 0xf0f0f0f0 >> 16 = 0xfffff0f0
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " >> "
             << hex0x32(rs2)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_srl(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    uint32_t rs2 = (uint32_t)this->regs.get(get_rs2(insn));

    // logical shift right
    int32_t res = (rs1 >> rs2);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, res);

    if (pos)
    {
        std::string s = render_rtype(insn, "srl     ");
        s.resize(instruction_width, ' ');
        // 000000d4: 00f751b3 srl x3,x14,x15 // x3 = 0xf0f0f0f0 >> 16 = 0x0000f0f0
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " >> "
             << hex0x32(rs2)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_sub(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t sub = rs1 - rs2;
    this->regs.set(reg, sub);

    if (pos)
    {
        std::string s = render_rtype(insn, " sub     ");
        s.resize(instruction_width, ' ');
        // 000000e0: 00f77233 and x4,x14,x15 // x4 = 0xf0f0f0f0 & 0xf0f0f0f0 = 0xf0f0f0f0
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " - "
             << hex0x32(rs2)
             << " = "
             << hex0x32(sub);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_xor(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t res = (rs1 ^ rs2);
    this->regs.set(reg, res);
    if (pos)
    {
        std::string s = render_rtype(insn, "xor     ");
        s.resize(instruction_width, ' ');
        // 000000d0: 00f74233 xor x4,x14,x15 // x4 = 0xf0f0f0f0 ^ 0xf0f0f0f0 = 0x00000000
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " ^ "
             << hex0x32(rs2)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

/*****************************************
 * I-Type Instructions
 * **************************************/
void rv32i::exec_addi(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t sum = rs1 + imm_i;
    this->regs.set(reg, sum);

    if (pos)
    {
        // 00000060: 01000313 addi x6,x0,16 // x6 = 0x00000000 + 0x00000010 = 0x00000010

        std::string s = render_itype_alu(insn, " addi   ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = " << hex0x32(rs1) << " + " << hex0x32(imm_i) << " = " << hex0x32(sum);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_andi(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t res = (rs1 & imm_i);
    this->regs.set(reg, res);

    if (pos)
    {
        //  000000ac: 4d267213 andi x4,x12,1234 // x4 = 0xf0f0f0f0 & 0x000004d2 = 0x000000d0

        std::string s = render_itype_alu(insn, " andi   ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = " << hex0x32(rs1) << " & " << hex0x32(imm_i) << " = " << hex0x32(res);
        *pos << std::endl;
    }
    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_jalr(uint32_t insn, std::ostream *pos)
{
    uint32_t reg = get_rd(insn);
    uint32_t nxt_insn = this->pc + 4;
    this->regs.set(reg, nxt_insn);

    int32_t rs1 = this->regs.get(get_rs1(insn));
    uint32_t imm_i = get_imm_i(insn);

    // jump
    this->pc = ((rs1 + imm_i) & 0xFFFFFFFE);

    if (pos)
    {
        // 00000010: 01008267 jalr x4,16(x1) // x4 = 0x00000014,
        //pc = (0x00000010 + 0x0000000c) & 0xfffffffe = 0x0000001c

        // TODO: find out  0x0000000c

        std::string s = render_jalr(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = " << hex0x32(nxt_insn) << ", pc"
             << " = (" << hex0x32(imm_i) << " + " << hex0x32(rs1) << ") & "
             << "0xfffffffe"
             << " = " << hex0x32(this->pc);
        *pos << std::endl;
    }
}
void rv32i::exec_lb(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    uint32_t addr = rs1 + imm_i;

    int32_t data = this->mem->get8(addr);

    // sign extend for 8-bit data
    // 1000 0000 & data
    if (0x80 & data)
        data |= 0xFFFFFF00;

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, data);

    if (pos)
    {

        std::string s = render_itype_load(insn, " lb   ");
        // 00000074: 01030203 lb x4,16(x6) // x4 = sx(m8(0x00000010 + 0x00000010)) = 0xffffffe3

        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = sx(m8(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}
void rv32i::exec_lh(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    uint32_t addr = rs1 + imm_i;
    int32_t data = this->mem->get16(addr);

    // sign extend
    if (0x8000 & data)
        data |= 0xFFFF0000;

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, data);

    if (pos)
    {
        std::string s = render_itype_load(insn, " lh   ");

        // 0000007c: 01031203 lh x4,16(x6) // x4 = sx(m16(0x00000010 + 0x00000010)) = 0x00004ae3
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = sx(m16(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}
void rv32i::exec_lw(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    uint32_t addr = rs1 + imm_i;

    int32_t data = this->mem->get32(addr);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, data);

    if (pos)
    {

        std::string s = render_itype_load(insn, " lw   ");

        // 00000084: 01032203 lw x4,16(x6) // x4 = sx(m32(0x00000010 + 0x00000010)) = 0xfe004ae3
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = sx(m32(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}
void rv32i::exec_lbu(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    uint32_t addr = rs1 + imm_i;

    // zero extended
    int32_t data = this->mem->get8(addr);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, data);

    if (pos)
    {

        std::string s = render_itype_load(insn, " lbu   ");
        // 00000064: 01034203 lbu x4,16(x6) // x4 = zx(m8(0x00000010 + 0x00000010)) = 0x000000e3

        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = zx(m8(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}
void rv32i::exec_lhu(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    uint32_t addr = rs1 + imm_i;

    // zero extended
    int32_t data = this->mem->get16(addr);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, data);

    if (pos)
    {

        std::string s = render_itype_load(insn, " lhu   ");

        // 0000006c: 01035203 lhu x4,16(x6) // x4 = zx(m16(0x00000010 + 0x00000010)) = 0x00004ae3
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "x" << reg << " = zx(m16(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_ori(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t res = (rs1 | imm_i);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, res);

    if (pos)
    {
        std::string s = render_itype_alu(insn, " ori     ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        // 000000a8: 4d266213 ori x4,x12,1234 // x4 = 0xf0f0f0f0 | 0x000004d2 = 0xf0f0f4f2
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " | "
             << hex0x32(imm_i)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_slli(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);

    // 5 LSB of imm_i is shamt_i
    int32_t shamt_i = (imm_i & 0x1F);

    int32_t res = (rs1 << shamt_i);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, res);

    if (pos)
    {
        std::string s = render_itype_alu(insn, " slli     ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        // 000000b0: 00c69213 slli x4,x13,12 // x4 = 0xf0f0f0f0 << 12 = 0x0f0f0000
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " << "
             << hex0x32(imm_i)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_slti(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, (rs1 < imm_i) ? 1 : 0);

    if (pos)
    {
        std::string s = render_itype_alu(insn, " slti     ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        // 0000009c: 4d262213 slti x4,x12,1234 // x4 = (0xf0f0f0f0 < 1234) ? 1 : 0 = 0x00000001
        *pos << s << "// "
             << "x" << get_rd(insn) << " = ("
             << hex0x32(rs1)
             << " <U "
             << hex0x32(imm_i)
             << ") ? 1 : 0 = "
             << hex0x32((rs1 < imm_i) ? 1 : 0);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_sltiu(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    uint32_t imm_i = get_imm_i(insn);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, (rs1 < imm_i) ? 1 : 0);

    if (pos)
    {
        std::string s = render_itype_alu(insn, " sltiu     ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        // 000000a0: 4d263213 sltiu x4,x12,1234 // x4 = (0xf0f0f0f0 <U 1234) ? 1 : 0 = 0x00000000
        *pos << s << "// "
             << "x" << get_rd(insn) << " = ("
             << hex0x32(rs1)
             << " <U "
             << hex0x32(imm_i)
             << ") ? 1 : 0 = "
             << hex0x32((rs1 < imm_i) ? 1 : 0);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_srai(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);

    // 5 LSB of imm_i is shamt_i
    int32_t shamt_i = (imm_i & 0x1F);

    // arithmetic shift right
    int32_t data = (rs1 >> shamt_i);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, data);

    if (pos)
    {
        std::string s = render_itype_alu(insn, " srai     ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        // 000000b8: 40c6d213 srai x4,x13,12 // x4 = 0xf0f0f0f0 >> 12 = 0xffff0f0f
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " >> "
             << hex0x32(imm_i)
             << " = "
             << hex0x32(data);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_srli(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);

    // 5 LSB of imm_i is shamt_i
    int32_t shamt_i = (imm_i & 0x1F);

    // logical shift right
    int32_t data = (rs1 >> shamt_i);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, data);

    if (pos)
    {
        std::string s = render_itype_alu(insn, " srli     ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        // 000000b4: 00c6d213 srli x4,x13,12 // x4 = 0xf0f0f0f0 >> 12 = 0x000f0f0f
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " >> "
             << hex0x32(imm_i)
             << " = "
             << hex0x32(data);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

void rv32i::exec_xori(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);

    // xor
    int32_t res = (rs1 ^ imm_i);

    uint32_t reg = get_rd(insn);
    this->regs.set(reg, res);

    if (pos)
    {
        std::string s = render_itype_alu(insn, " srli     ", get_imm_i(insn));
        s.resize(instruction_width, ' ');
        // 000000a4: 4d264213 xori x4,x12,1234 // x4 = 0xf0f0f0f0 ^ 0x000004d2 = 0xf0f0f422
        *pos << s << "// "
             << "x" << get_rd(insn) << " = "
             << hex0x32(rs1)
             << " ^ "
             << hex0x32(imm_i)
             << " = "
             << hex0x32(res);
    }

    // increment program counter
    this->pc = this->pc + 4;
}

/*****************************************
 * S-Type Instructions
 * **************************************/
void rv32i::exec_sb(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_s = get_imm_s(insn);
    uint32_t addr = rs1 + imm_s;
    uint8_t data = (uint8_t)this->regs.get(get_rs2(insn));
    this->mem->set8(addr, data);

    if (pos)
    {

        std::string s = render_stype(insn, " sb     ");
        // 0000008c: 0e500ea3 sb x5,253(x0) // m8(0x00000000 + 0x000000fd) = 0x000000ff

        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "m8(" << hex0x32(rs1) << " + " << hex0x32(imm_s) << ") = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}
void rv32i::exec_sh(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_s = get_imm_s(insn);
    uint32_t addr = rs1 + imm_s;
    uint16_t data = (uint16_t)this->regs.get(get_rs2(insn));
    this->mem->set8(addr, data);

    if (pos)
    {

        std::string s = render_stype(insn, " sh     ");
        // 00000090: 0e501823 sh x5,240(x0) // m16(0x00000000 + 0x000000f0) = 0x0000ffff
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "m16(" << hex0x32(rs1) << " + " << hex0x32(imm_s) << ") = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}
void rv32i::exec_sw(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t imm_s = get_imm_s(insn);
    uint32_t addr = rs1 + imm_s;
    uint32_t data = (uint32_t)get_rs2(insn);
    this->mem->set8(addr, data);

    if (pos)
    {

        std::string s = render_stype(insn, " sh     ");
        // 00000094: 0e502a23 sw x5,244(x0) // m32(0x00000000 + 0x000000f4) = 0xffffffff
        s.resize(instruction_width, ' ');
        *pos << s << "//  ";
        *pos << "m32(" << hex0x32(rs1) << " + " << hex0x32(imm_s) << ") = " << hex0x32(data);
        *pos << std::endl;
    }

    // increment program counter
    this->pc = this->pc + 4;
}

/*****************************************
 * B-Type Instructions
 * **************************************/
void rv32i::exec_beq(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t imm_b = get_imm_b(insn);

    // conditional jump
    this->pc += (rs1 == rs2 ? imm_b : 4);

    if (pos)
    {
        std::string s = render_btype(insn, " beq    ");
        s.resize(instruction_width, ' ');
        // 00000030: 00000463 beq x0,x0,0x38 // pc += (0x00000000 == 0x00000000 ? 0x00000008 : 4) = 0x00000038
        *pos << s << "//  ";

        *pos << "pc += (" << hex0x32(rs1) << " == " << hex0x32(rs2) << " ? "
             << hex0x32(imm_b) << " : "
             << "4"
             << ") = " << hex0x32(this->pc);
        *pos << std::endl;
    }
}
void rv32i::exec_bge(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t imm_b = get_imm_b(insn);

    // conditional jump
    this->pc += (rs1 >= rs2 ? imm_b : 4);

    if (pos)
    {
        std::string s = render_btype(insn, " bge    ");
        s.resize(instruction_width, ' ');
        // 00000024: fe0558e3 bge x10,x0,0x14 //
        // pc += (0xf0f0f0f0 >= 0x00000000 ? 0xfffffff0 : 4) = 0x00000028
        *pos << s << "//  ";

        *pos << "pc += (" << hex0x32(rs1) << " >= " << hex0x32(rs2) << " ? "
             << hex0x32(imm_b) << " : "
             << "4"
             << ") = " << hex0x32(this->pc);
        *pos << std::endl;
    }
}
void rv32i::exec_bgeu(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    uint32_t rs2 = (uint32_t)this->regs.get(get_rs2(insn));
    int32_t imm_b = get_imm_b(insn);

    // conditional jump
    this->pc += (rs1 >= rs2 ? imm_b : 4);

    if (pos)
    {
        std::string s = render_btype(insn, " bgeu    ");
        s.resize(instruction_width, ' ');
        // 0000002c: fea074e3 bgeu x0,x10,0x14 // pc += (0x00000000 >=U 0xf0f0f0f0 ? 0xffffffe8 : 4) = 0x00000030
        *pos << s << "//  ";

        *pos << "pc += (" << hex0x32(rs1) << " >=U " << hex0x32(rs2) << " ? "
             << hex0x32(imm_b) << " : "
             << "4"
             << ") = " << hex0x32(this->pc);
        *pos << std::endl;
    }
}
void rv32i::exec_blt(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t imm_b = get_imm_b(insn);

    // conditional jump
    this->pc += (rs1 < rs2 ? imm_b : 4);

    if (pos)
    {
        std::string s = render_btype(insn, " blt    ");
        s.resize(instruction_width, ' ');
        // 00000020: fe004ae3 blt x0,x0,0x14 //
        // pc += (0x00000000 < 0x00000000 ? 0xfffffff4 : 4) = 0x00000024
        *pos << s << "//  ";

        *pos << "pc += (" << hex0x32(rs1) << " < " << hex0x32(rs2) << " ? "
             << hex0x32(imm_b) << " : "
             << "4"
             << ") = " << hex0x32(this->pc);
        *pos << std::endl;
    }
}
void rv32i::exec_bltu(uint32_t insn, std::ostream *pos)
{
    uint32_t rs1 = (uint32_t)this->regs.get(get_rs1(insn));
    uint32_t rs2 = (uint32_t)this->regs.get(get_rs2(insn));
    int32_t imm_b = get_imm_b(insn);

    // conditional jump
    this->pc += (rs1 < rs2 ? imm_b : 4);

    if (pos)
    {
        std::string s = render_btype(insn, " bge    ");
        s.resize(instruction_width, ' ');
        // 00000028: fe0066e3 bltu x0,x0,0x14 // pc += (0x00000000 <U 0x00000000 ? 0xffffffec : 4) = 0x0000002c
        *pos << s << "//  ";

        *pos << "pc += (" << hex0x32(rs1) << " <U " << hex0x32(rs2) << " ? "
             << hex0x32(imm_b) << " : "
             << "4"
             << ") = " << hex0x32(this->pc);
        *pos << std::endl;
    }
}
void rv32i::exec_bne(uint32_t insn, std::ostream *pos)
{
    int32_t rs1 = this->regs.get(get_rs1(insn));
    int32_t rs2 = this->regs.get(get_rs2(insn));
    int32_t imm_b = get_imm_b(insn);

    // conditional jump
    this->pc += (rs1 != rs2 ? imm_b : 4);

    if (pos)
    {
        std::string s = render_btype(insn, " bne    ");
        s.resize(instruction_width, ' ');
        // s0000001c: feb59ce3 bne x11,x11,0x14
        // // pc += (0xf0f0f0f0 != 0xf0f0f0f0 ? 0xfffffff8 : 4) = 0x00000020

        *pos << s << "//  ";

        *pos << "pc += (" << hex0x32(rs1) << " != " << hex0x32(rs2) << " ? "
             << hex0x32(imm_b) << " : "
             << "4"
             << ") = " << hex0x32(this->pc);
        *pos << std::endl;
    }
}

// Instruction Executions Done

void rv32i::exec_btype(uint32_t insn, const char *mnemonic, std::ostream *pos)
{
}
void rv32i::exec_itype_load(uint32_t insn, const char *mnemonic, std::ostream *pos)
{
}
void rv32i::exec_stype(uint32_t insn, const char *mnemonic, std::ostream *pos)
{
}
void rv32i::exec_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i, std::ostream *pos)
{
}
void rv32i::exec_rtype(uint32_t insn, const char *mnemonic, std::ostream *pos)
{
}
void rv32i::exec_fence(uint32_t insn, std::ostream *pos)
{
}
void rv32i::exec_ecall(uint32_t insn, std::ostream *pos)
{
}

void rv32i::exec_eror(uint32_t insn, std::ostream *pos)
{
}

/*****************************************
 * String render formatting functions
 * **************************************/

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
    os << " lui     x" << std::dec << get_rd(insn) << ",0x" << std::hex << (get_imm_u(insn) >> 12);
    return os.str();
}
std::string rv32i::render_auipc(uint32_t insn) const
{
    std::ostringstream os;

    os << hex32(insn) << " "; // the instruction hex value
    os << " auipc   x" << std::dec << get_rd(insn) << ",0x" << std::hex << (get_imm_u(insn) >> 12);

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

std::string rv32i::render_total_insn_exec(uint64_t total) const
{
    std::ostringstream os;
    os << this->insn_counter;
    if (total == 1)
        os << " instruction executed";
    else
        os << " instructions executed";
    return os.str();
}
#ifndef rv32i_H
#define rv32i_H

#include <cstdint>
#include <string>
#include "memory.h"
#include "registerfile.h"

// static definitions
static constexpr int mnemonic_width = 8;
static constexpr int instruction_width = 35;
static constexpr uint32_t opcode_lui = 0b0110111;
static constexpr uint32_t opcode_auipc = 0b0010111;
static constexpr uint32_t opcode_jal = 0b1101111;
static constexpr uint32_t opcode_jalr = 0b1100111;
static constexpr uint32_t opcode_btype = 0b1100011;
static constexpr uint32_t opcode_stype = 0b0100011;
static constexpr uint32_t opcode_rtype = 0b0110011;
static constexpr uint32_t opcode_load_imm = 0b0000011;
static constexpr uint32_t opcode_alu_imm = 0b0010011;
static constexpr uint32_t opcode_fenc_opt = 0b0001111;
static constexpr uint32_t opcode_exc = 0b1110011;

class rv32i
{
private:
    // Member variables from Assignment 4
    memory *mem;
    uint32_t pc;
    static constexpr uint32_t XLEN = 32;

    // Member variables from Assignment 5
    registerfile regs;
    bool halt;
    bool show_instructions;
    bool show_registers;
    uint64_t insn_counter;

public:
    // Member functions from Assignment 4
    // Save the m argument in the mem member variable for use later when disassembling
    rv32i(memory *m);

    // This method will be used to disassemble the instructions in the simulated memory
    // To perform this task, set pc to zero and then, for each 32-bit word in the memory
    void disasm(void);

    // This function must be capable of handling any possible insn value.
    // It is the purpose of this function to return a std::string containing
    // the disassembled instruction text. This function will not print anything.
    std::string decode(uint32_t insn) const;

    // Static Member functions from Assignment 4

    // Extract and return the opcode field from the given instruction
    static uint32_t get_opcode(uint32_t insn);

    // Extract and return the rd field from the given instruction.
    static uint32_t get_rd(uint32_t insn);

    // Extract and return the funct3 field from the given instruction
    static uint32_t get_funct3(uint32_t insn);

    // Extract and return the rs1 field from the given instruction
    static uint32_t get_rs1(uint32_t insn);

    // Extract and return the rs2 field from the given instruction
    static uint32_t get_rs2(uint32_t insn);

    // Extract and return the funct7 field from the given instruction
    static uint32_t get_funct7(uint32_t insn);

    // Extract and return the imm_i field from the given instruction
    static int32_t get_imm_i(uint32_t insn);

    // Extract and return the imm_u field from the given instruction static int32_t get_imm_u(uint32_t insn);
    static int32_t get_imm_u(uint32_t insn);

    // Extract and return the imm_b field from the given instruction
    static int32_t get_imm_b(uint32_t insn);

    // Extract and return the imm_s field from the given instruction
    static int32_t get_imm_s(uint32_t insn);

    // Extract and return the imm_j field from the given instruction
    static int32_t get_imm_j(uint32_t insn);

    // Member functions from Assignment 5
    void set_show_instructions(bool b);
    void set_show_registers(bool b);
    bool is_halted() const;
    void reset();
    void dump() const;

    // function to execute individual instruction
    void tick();

    // function to loop through instruction set
    void run(uint64_t limit);

    // Instruction Execution functions
    void dcex(uint32_t insn, std::ostream *);
    void exec_illegal_insn(uint32_t insn, std::ostream *pos);
    void exec_ebreak(uint32_t insn, std::ostream *pos);
    // void exec_btype(uint32_t insn, const char *mnemonic, std::ostream *pos);
    // void exec_itype_load(uint32_t insn, const char *mnemonic, std::ostream *pos);
    // void exec_stype(uint32_t insn, const char *mnemonic, std::ostream *pos);
    // void exec_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i, std::ostream *pos);
    // void exec_rtype(uint32_t insn, const char *mnemonic, std::ostream *pos);
    void exec_fence(uint32_t insn, std::ostream *pos);
    void exec_ecall(uint32_t insn, std::ostream *pos);
    void exec_error(uint32_t insn, std::ostream *pos);

    // U-Type Instructions
    void exec_lui(uint32_t insn, std::ostream *pos);
    void exec_auipc(uint32_t insn, std::ostream *pos);

    // J-Type Instructions
    void exec_jal(uint32_t insn, std::ostream *pos);

    // R-Type Instructions
    void exec_add(uint32_t insn, std::ostream *pos);
    void exec_and(uint32_t insn, std::ostream *pos);
    void exec_or(uint32_t insn, std::ostream *pos);
    void exec_sll(uint32_t insn, std::ostream *pos);
    void exec_slt(uint32_t insn, std::ostream *pos);
    void exec_sltu(uint32_t insn, std::ostream *pos);
    void exec_sra(uint32_t insn, std::ostream *pos);
    void exec_srl(uint32_t insn, std::ostream *pos);
    void exec_sub(uint32_t insn, std::ostream *pos);
    void exec_xor(uint32_t insn, std::ostream *pos);

    // I-Type Instructions
    void exec_addi(uint32_t insn, std::ostream *pos);
    void exec_andi(uint32_t insn, std::ostream *pos);
    void exec_jalr(uint32_t insn, std::ostream *pos);
    void exec_lb(uint32_t insn, std::ostream *pos);
    void exec_lh(uint32_t insn, std::ostream *pos);
    void exec_lw(uint32_t insn, std::ostream *pos);
    void exec_lbu(uint32_t insn, std::ostream *pos);
    void exec_lhu(uint32_t insn, std::ostream *pos);
    void exec_ori(uint32_t insn, std::ostream *pos);
    void exec_slli(uint32_t insn, std::ostream *pos);
    void exec_slti(uint32_t insn, std::ostream *pos);
    void exec_sltiu(uint32_t insn, std::ostream *pos);
    void exec_srai(uint32_t insn, std::ostream *pos);
    void exec_srli(uint32_t insn, std::ostream *pos);
    void exec_xori(uint32_t insn, std::ostream *pos);

    // S-Type Instructions
    void exec_sb(uint32_t insn, std::ostream *pos);
    void exec_sh(uint32_t insn, std::ostream *pos);
    void exec_sw(uint32_t insn, std::ostream *pos);

    // B-Type Instructions
    void exec_beq(uint32_t insn, std::ostream *pos);
    void exec_bge(uint32_t insn, std::ostream *pos);
    void exec_bgeu(uint32_t insn, std::ostream *pos);
    void exec_blt(uint32_t insn, std::ostream *pos);
    void exec_bltu(uint32_t insn, std::ostream *pos);
    void exec_bne(uint32_t insn, std::ostream *pos);

    // String render formatting
    std::string render_illegal_insn(uint32_t insn) const;
    std::string render_lui(uint32_t insn) const;
    std::string render_auipc(uint32_t insn) const;
    std::string render_jal(uint32_t insn) const;
    std::string render_jalr(uint32_t insn) const;
    std::string render_btype(uint32_t insn, const char *mnemonic) const;
    std::string render_itype_load(uint32_t insn, const char *mnemonic) const;
    std::string render_stype(uint32_t insn, const char *mnemonic) const;
    std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const;
    std::string render_rtype(uint32_t insn, const char *mnemonic) const;
    std::string render_fence(uint32_t insn) const;
    std::string render_ecall(uint32_t insn) const;
    std::string render_ebreak(uint32_t insn) const;
    std::string render_eror(uint32_t insn) const;
    std::string render_total_insn_exec(uint64_t total) const;
};

#endif // rv32i_H
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
static constexpr uint32_t opcode_load_imm = 0b0000011;

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
    uint32_t pc;

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
    static uint32_t get_opcode(uint32_t insn);
    static uint32_t get_rd(uint32_t insn);
    static uint32_t get_funct3(uint32_t insn);
    static uint32_t get_rs1(uint32_t insn);
    static uint32_t get_rs2(uint32_t insn);
    static uint32_t get_funct7(uint32_t insn);
    static int32_t get_imm_i(uint32_t insn);
    static int32_t get_imm_u(uint32_t insn);
    static int32_t get_imm_b(uint32_t insn);
    static int32_t get_imm_s(uint32_t insn);
    static int32_t get_imm_j(uint32_t insn);

    // Member functions from Assignment 5
    void set_show_instructions(bool b);
    void set_show_registers(bool b);
    bool is_halted() const;
    void reset();
    void dump() const;
    void dcex(uint32_t insn, std::ostream *);
    void exec_illegal_insn(uint32_t insn, std::ostream *pos);
    void exec_xxx(uint32_t insn, std::ostream *);
    void tick();
    void run(uint64_t limit);
};
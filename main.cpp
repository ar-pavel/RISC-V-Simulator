#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include "hex.h"
#include "memory.h"
#include "rv32i.h"

/**
 * Print a usage message and abort the program.
 *********************************************************************/
static void usage()
{
    std::cerr << "Usage: rv32i [-m hex-mem-size] infile" << std::endl;
    std::cerr << "    -m specify memory size (default = 0x10000)" << std::endl;
    exit(1);
}

/**
 * Read a file of RV32I instructions and execute them.
 ********************************************************************/
int main(int argc, char **argv)
{
    uint32_t memory_limit = 0x10000; // default memory size = 64k
    uint64_t execution_limit = 0;    // 0 = run forever
    int opt;

    bool show_disasm = false;
    bool show_insn = false;
    bool show_regs = false;
    bool show_mem_dump = false;

    while ((opt = getopt(argc, argv, "dil::m::rz")) != -1)
    {
        switch (opt)
        {
        case 'd':
            show_disasm = true;
            // std::cout << " found d at \n";
            break;
        case 'i':
            // std::cout << " found i at \n";
            show_insn = true;
            break;
        case 'l':
            // std::cout << " found l at \n";
            execution_limit = (uint32_t)std::stoul(optarg, nullptr, 10);
            // std::cout << " execution limit: " << execution_limit << "\n";
            break;
        case 'm':
            // hex_mem_size given as argument is assigned to memory limit
            memory_limit = (uint32_t)std::stoul(optarg, nullptr, 16);
            // std::cout << " found m at \n";
            // std::cout << " memory limit: " << memory_limit << "\n";
            break;
        case 'r':
            // std::cout << " found r at \n";
            show_regs = true;
            break;
        case 'z':
            // std::cout << " found z at \n";
            show_mem_dump = true;
            break;

        default:
            /* '?' */
            usage();
        }
    }
    // std::cout << " reading instructions from: " << argv[argc - 1] << std::endl;
    // if (3 >= argc)
    //     usage(); // missing filename

    memory mem(memory_limit);

    // missing filename or file loading error
    if (!mem.load_file(argv[argc - 1]))
        usage();

    rv32i sim(&mem);

    // print instructions if option -i is given
    sim.set_show_instructions(show_insn);

    // print register hart dump if option -r is given
    sim.set_show_registers(show_regs);

    // run the simulated with fixed limit if -l flag has an argument
    sim.run(execution_limit);

    // disassemble the instructions
    if (show_disasm)
        sim.disasm();

    // show memory dump
    if (show_mem_dump)
        mem.dump();
    return 0;
}

#include "hex.h"

#include "memory.h"

#include "rv32i.h"

#include <stdlib.h>

#include <iostream>

#include <unistd.h>

using namespace std;

/**
 * Print a usage message and abort the program.
 *********************************************************************/
static void usage()
{
    cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
    cerr << "    -m specify memory size (default = 0x10000)" << endl;
    exit(1);
}

/**
 * Read a file of RV32I instructions and execute them.
 ********************************************************************/
int main(int argc, char **argv)
{
    uint32_t memory_limit = 0x10000; // default memory size = 64k
    int opt;

    while ((opt = getopt(argc, argv, "m:")) != -1)
    {
        switch (opt)
        {
        case 'm':
            memory_limit = std::stoul(argv[2], nullptr, 16);
            break;
        default:
            /* '?' */
            usage();
        }
    }

    if (3 >= argc)
        usage(); // missing filename

    memory mem(memory_limit);

    if (!mem.load_file(argv[3]))
        usage();

    rv32i sim(&mem);
    sim.disasm();
    mem.dump();

    return 0;
}
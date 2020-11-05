#include <iostream>
#include <fstream>
#include "hex.h"
#include "memory.h"
/*
Use: initializes vector to 0xa5
Parameters: 1. uint32_t siz: used to determined vectors size
*/
memory::memory(uint32_t siz)
{
    size = (siz + 15) & 0xfffffff0;

    //resizes vector to size value and sets every element to 0xa5
    mem.resize(size, 0xa5);
}

/*
Use: calls destructor for vector
Parameters: none
*/
memory::~memory()
{
    //destructor
    mem.clear();
}

/*
Use: checks if address is valid
Parameters: 1. uint32_t i: address to be checked
*/
bool memory::check_address(uint32_t i) const
{
    //checks if i is less or equal to size
    if (i < size)
    {
        return true;
    }
    //print warning if address out of range
    else
    {
        std::cout << "WARNING: Address out of range: " << hex0x32(i) << std::endl;
        return 0;
    }
}

/*
Use: returns size
Parameters: none
*/
uint32_t memory::get_size() const
{
    return size;
}

/*
Use: returns value in a given address
Parameters: 1. uint32_t: used for getting value in a certain address

*/
uint8_t memory::get8(uint32_t addr) const
{
    //checks if check address is true
    if (check_address(addr))
    {
        return mem[addr];
    }
    else
    {
        return 0;
    }
}

/*


Use: returns value in a given address (little endiend format)
Parameters: 1. uint16_t: used for getting value in a certain address

*/
uint16_t memory::get16(uint32_t addr) const
{
    uint16_t sum;
    //sets sum to the addresses in little endiend format
    sum = (get8(addr) | get8(addr + 1) << 8);
    return sum;
}

/*
Use: returns value in a given address (little endiend format)
Parameters: 1. uint32_t: used for getting value in a certain address
*/
uint32_t memory::get32(uint32_t addr) const
{
    uint32_t sum;
    //sets sum to the addresses in little endiend format
    sum = get16(addr) | get16(addr + 2) << 16;
    return sum;
}

/*
Use: sets val to the given address
Parameters: 1. uint32_t: used to determined the address
*/
void memory::set8(uint32_t addr, uint8_t val)
{
    //checks if address is valid
    if (check_address(addr))
    {
        mem[addr] = val;
    }
}

/*
Use: sets val to the given address
Parameters: 1. uint32_t: used to determined the address
* 			2. uint16_t: value to be stored (little endiend)
*/
void memory::set16(uint32_t addr, uint16_t val)
{
    //shifts value 8 bits address and increments address
    set8(addr + 1, val >> 8);
    set8(addr, val);
    mem[addr] = val;
}

/*


Use: sets val to the given address
Parameters: 1. uint32_t: used to determined the address
* 			2. unint32_t: value to be stored (little endiend)

*/
void memory::dump() const
{
    char ascii[17];
    ascii[16] = 0;
    for (uint32_t i = 0; i < size; i++)
    {
        if (i % 16 == 0)
        {
            if (i != 0)
                std::cout << " *" << ascii << "*" << std::endl;
            std::cout << hex32(i) << ":";
        }
        uint8_t ch = get8(i);
        std::cout << (i % 16 == 8 ? "  " : " ") << hex8(ch);
        ascii[i % 16] = isprint(ch) ? ch : '.';
    }
    std::cout << " *" << ascii << "*" << std::endl;
}

bool memory::load_file(const std::string &fname)
{
    char temp;
    int counter = 0;
    std::ifstream infile(fname, std::ios::in | std::ios::binary);
    //checks if file exists or can be opened
    if (!infile)
    {
        std::cout << "Can't open file " << fname << " for reading" << std::endl;
        return false;
    }
    //reads in bit by bit
    while (infile.get(temp))
    {
        //checks if address is ok
        if (check_address(counter))
        {
            //sets value of temp into vector index (counter)
            mem[counter] = temp;
        }
        else
        {
            std::cerr << "Program too big." << std::endl;
            return false;
        }
        counter++;
    }
    return true;
}

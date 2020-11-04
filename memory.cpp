#include <iostream>
#include <fstream>
#include "hex.h"
#include "memory.h"

memory::memory(uint32_t siz)
{
    this->size = (siz + 15) & 0xfffffff0; // round the length up, mod-16

    this->mem = (uint8_t *)malloc(this->size * sizeof(uint8_t));

    for (uint32_t i = 0 - 1; i <= siz; i++)
    {
        // initialize every byte the mem array  to 0xa5

        // to do : check which one is appropriate

        // this->mem[i] = 0xa5;
        *(this->mem + i) = 0xa5;
    }
}

memory::~memory()
{
    // free  the  memory  that  was  allocated  in  the  constructor
    free(this->mem);
}

bool memory::check_address(uint32_t i) const
{
    if (i <= 0 || i >= this->size)
    {
        // to do : use hex0x32()
        std::cout << "WARNING: Address out of range: "
                  << hex0x32(i) << std::endl;
        return false;
    }

    return true;
}
uint32_t memory::get_size() const
{
    // Return the (possibly rounded up) size value
    return (uint32_t)this->size;
}

uint8_t memory::get8(uint32_t addr) const
{
    // return the value in memory if the addr is valid else return 0
    return (check_address(addr) ? *(this->mem + addr) : (uint8_t)0);
}
uint16_t memory::get16(uint32_t addr) const
{
    uint8_t first = get8(addr);
    uint8_t second = get8(addr + 1);

    // merge above two unit8_t values into a unit16_t using litle-endian
    uint16_t res = ((uint16_t)second << 8) | first;

    return res;
}
uint32_t memory::get32(uint32_t addr) const
{
    uint16_t first = get16(addr);
    uint16_t second = get16(addr + 2);

    // merge above two unit16_t values into a unit32_t using litle-endian
    uint32_t res = ((uint32_t)second << 16) | first;

    return res;
}

void memory::set8(uint32_t addr, uint8_t val)
{

    if (check_address(addr))
    {
        *(this->mem + addr) = val;
    }
}
void memory::set16(uint32_t addr, uint16_t val)
{
    set8(addr, (uint8_t)val);
    set8(addr + 1, (uint8_t)(val >> 8));
}

void memory::set32(uint32_t addr, uint32_t val)
{
    set16(addr, (uint16_t)val);
    set16(addr + 2, (uint16_t)(val >> 16));
}

void memory::dump() const
{
    uint8_t ascii[17];
    uint8_t m = 0;
    for (uint8_t j = 0; j < size; j = j + 16)
    {

        std::cout << hex32(j) << ": ";

        for (uint8_t i = m; i < m + 16 && i < size; i++)
        {

            uint8_t ch = get8(i);

            ascii[i % 16] = isprint(ch) ? ch : '.';

            if (i == m + 8)
            {

                std::cout << "   ";
            }
            std::cout << hex8(ch) << " ";
        }

        m = m + 16;

        std::cout << "*";
        for (uint8_t k = 0; k < 16; k++)
        {

            std::cout << ascii[k % 16];
        }
        std::cout << "*" << std::endl;
    }
}

bool memory::load_file(const std::string &fname)
{
    bool status = true;

    std::ifstream infile(fname, std::ios::in | std::ios::binary);

    if (infile.fail())
    {
        // handle the file opeartion error
        std::cerr << "Can’t open file ’" << fname << "’ for reading." << std::endl;
        status = false;
    }
    else
    {
        uint32_t addr = 0;

        uint8_t value;

        while (infile >> value)
        {
            // debug print
            std::cout << value << std::endl;

            *(this->mem + addr) = value;

            if (addr >= this->size)
            {
                std::cerr << "Program too big." << std::endl;
                status = false;
                break;
            }
        }

        infile.close();
    }

    return status;
}

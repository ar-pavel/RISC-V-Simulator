#include <iostream>
#include <iomanip>
#include "../include/registerfile.h"

// Initialize register x0 to zero, and all other registers to 0xf0f0f0f0.
void registerfile::reset()
{
    // assigning zero (0) to x0
    this->registers[0] = 0;
    // assigning 0xf0f0f0f0 to every other registers
    for (int i = 1; i < 32; i++)
    {
        this->registers[i] = 0xf0f0f0f0;
    }
}

// Assign register r the given val. If r is zero, then do nothing.
void registerfile::set(uint32_t r, int32_t val)
{
    // if r = 0, do nothing
    // also make sure r is within valid range
    if (r > 0 && r < 32)
    {
        this->registers[r] = val;
    }
}

// Return the value of register r. If r is zero, then return zero
int32_t registerfile::get(uint32_t r) const
{
    // handle error
    if (r < 0 && r >= 32)
        return -1;

    // returning zero if r = 0
    if (r == 0)
        return 0;
    // otherwise returning value stored in the r-th register
    return this->registers[r];
}

/**
 * Implementation of a dump of the registers with the following format:
 * x0 00000000 f0f0f0f0 00001000 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
 * x8 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
 * x16 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
 * x24 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
 * **/
void registerfile::dump() const
{
    // registers from [0-7]
    std::cout << std::setfill(' ') << std::right << std::setw(3) << "x0";
    for (int i = 0; i < 8; i++)
    {
        std::cout << " " << std::setfill('0') << std::setw(8) << std::hex << this->registers[i];
    }
    std::cout << "\n";

    // registers from [8-15]
    std::cout << std::setfill(' ') << std::right << std::setw(3) << "x8";
    for (int i = 8; i < 16; i++)
    {
        std::cout << " " << std::setfill('0') << std::setw(8) << std::hex << this->registers[i];
    }
    std::cout << "\n";

    // registers from [16-23]
    std::cout << std::setfill(' ') << std::right << std::setw(3) << "x16";
    for (int i = 16; i < 24; i++)
    {
        std::cout << " " << std::setfill('0') << std::setw(8) << std::hex << this->registers[i];
    }
    std::cout << "\n";

    // registers from [24-32]
    std::cout << std::setfill(' ') << std::right << std::setw(3) << "x24";
    for (int i = 24; i < 32; i++)
    {
        std::cout << " " << std::setfill('0') << std::setw(8) << std::hex << this->registers[i];
    }
    std::cout << "\n";
}
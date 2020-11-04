#include "hex.h"
#include <iomanip>
#include <sstream>

// This function will return a std::string with exactly 2 hex digits representing the 8 bits of
// the i argument.
std::string hex8(uint8_t i)
{
    std::ostringstream os;
    // static_cast is necessary to prevent the insertion operator << from treating the 8-bit
    // integer as a character and printing it incorrectly
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}

// This function will return a std::string with 8 hex digits representing the 32 bits of the i argument.
std::string hex32(uint32_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << i;
    return os.str();
}

// This function will return a std::string beginning with 0x, followed by the 8 hex digits
// representing the 32 bits of the i argument
std::string hex0x32(uint32_t i)
{
    return std::string("0x") + hex32(i);
}
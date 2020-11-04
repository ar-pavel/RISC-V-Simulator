#ifndef hex_H
#define hex_H

#include <string>
#include <stdint.h>

// This function will return a std::string with exactly 2 hex digits representing the 8 bits of
// the i argument.
std::string hex8(uint8_t i);

// This function will return a std::string with 8 hex digits representing the 32 bits of the i
// argument.
std::string hex32(uint32_t i);

// This function will return a std::string beginning with 0x, followed by the 8 hex digits
// representing the 32 bits of the i argument
std::string hex0x32(uint32_t i);
#endif
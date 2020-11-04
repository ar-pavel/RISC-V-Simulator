#include <iostream>
#include "../include/memory.h"

memory::memory(uint32_t siz)
{
}

memory::~memory()
{
}

bool memory::check_address(uint32_t i) const
{
}
uint32_t memory::get_size() const
{
}

uint8_t memory::get8(uint32_t addr) const
{
}
uint16_t memory::get16(uint32_t addr) const
{
}
uint32_t memory::get32(uint32_t addr) const
{
}

void memory::set8(uint32_t addr, uint8_t val)
{
}
void memory::set16(uint32_t addr, uint16_t val)
{
}
void memory::set32(uint32_t addr, uint32_t val)
{
}

void memory::dump() const
{
}

bool memory::load_file(const std::string &fname)
{
}

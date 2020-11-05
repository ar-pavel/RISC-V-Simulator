#ifndef memory_H
#define memory_H

#include <cstdint>
#include <string>
#include <vector>
#include "hex.h"

class memory
{

public:
    memory(uint32_t siz);
    ~memory();

    bool check_address(uint32_t i) const; //checks address prototype
    uint32_t get_size() const;            //get_size prototype
    uint8_t get8(uint32_t addr) const;    //get8 prototype
    uint16_t get16(uint32_t addr) const;  //get16 prototype
    uint32_t get32(uint32_t addr) const;  //get32 prototype

    void set8(uint32_t addr, uint8_t val);   //set8 prototype
    void set16(uint32_t addr, uint16_t val); //set16 prototype
    void set32(uint32_t addr, uint32_t val); //set32 prototype

    void dump() const; //dump prototype

    bool load_file(const std::string &fname); //load_file prototype

private:
    // uint8_t *mem; // the  actual  memory  buffer

    std::vector<uint8_t> mem; // the actual memory buffer
    uint32_t size;            //size prototype
};

#endif // memory_H
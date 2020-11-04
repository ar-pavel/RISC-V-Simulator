#include <cstdint>

class registerfile
{
private:
    // array to hold 32 register values
    int32_t registers[32];

public:
    // initialize register x0 to zero, and all other registers to 0xf0f0f0f0
    void reset();
    // Assign register r the given val. If r is zero, then do nothing.
    void set(uint32_t r, int32_t val);
    // Return the value of register r. If r is zero, then return zero.
    int32_t get(uint32_t r) const;
    /**
     * Implement a dump of the registers with the following format:
     * x0 00000000 f0f0f0f0 00001000 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
     * x8 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
     * x16 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
     * x24 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
     * **/
    void dump() const;
};

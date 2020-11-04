#include <iostream>
#include "lib/registerfile.cpp"
#include "lib/hex.cpp"

int main()
{
    registerfile reg;
    reg.reset();
    reg.dump();
    // test commit
    return 0;
}
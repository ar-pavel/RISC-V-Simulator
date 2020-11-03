#include <iostream>
#include "lib/registerfile.cpp"

int main()
{
    registerfile reg;
    reg.reset();
    reg.dump();
    return 0;
}
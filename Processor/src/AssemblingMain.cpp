#include "Assembling.h"
#include "Types.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    c_string filename = argv[1];

    if (filename == nullptr)
        doAssembler("src/input.bin");
    else
        doAssembler(mkInputDir(filename));

    return 0;
}
#include "Assembling.h"
#include "Types.h"
#include <stdio.h>
#include <stdlib.h>

#pragma GCC diagnostic warning "-Wunused-variable"
#pragma GCC diagnostic warning "-Wwrite-strings"

int main(int argc, char* argv[])
{
    c_string filename = argv[1];

    if (filename == nullptr)
        makeCompile("input.bin");
    else
        makeCompile(mkInputDir(filename));
    
    return 0;
}
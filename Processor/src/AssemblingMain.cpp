#include "Assembling.h"
#include "Types.h"
#include <stdio.h>
#include <stdlib.h>
#include "ProcessorCompilerCfg.h"
#include "Lexer.h"

int main(int argc, char* argv[])
{
    c_string filename = argv[1];

    if (filename == nullptr)
        makeCompile("src/input.bin");
    else
        makeCompile(mkInputDir(filename));

    return 0;
}
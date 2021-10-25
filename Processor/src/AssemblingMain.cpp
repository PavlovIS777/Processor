#include "Assembling.h"
#include "Types.h"
#include <stdio.h>



int main(int argc, char* argv[])
{
    c_string filename = argv[1];
    int stringsCount = 0;
    
    c_string assembledStr = nullptr;
    if (filename == nullptr)
        assembledStr = doAssembler(&stringsCount, "src/input.bin");
    else
        assembledStr = doAssembler(&stringsCount, mkInputDir(filename));
    
    printf("%s", assembledStr);
    
    
    return 0;
}
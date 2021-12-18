#ifndef INVOKER_H
#define INVOKER_H

#include "Types.h"
#include "stdint.h"
#include "StackOverflow.h"

#define MAX_MEM_SIZE 1<<14
#define SKIP_ZERORESULT_CMD_FLAG 0 ^ 255

struct Registers
{
    int ax;
    int bx;
    int cx;
    int dx;
    int ex;
};

struct Invoker
{   
    c_string compiledCode;
    int codeBytesLen;
    int* memory;
    int memPos;
    int videomemPos;
    Registers registers; 
    int ip;
    MyStack stack;
};

#endif //!INVOKER_H
#ifndef INVOKER_H
#define INVOKER_H

#include "Types.h"
#include "stdint.h"
#include "StackOverflow.h"

#define MAX_MEM_SIZE 1<<14

struct Registers
{
    int ax;
    int bx;
    int cx;
    int dx;
};

struct Invoker
{   
    c_string compiledCode;
    int* memory;
    int memPos;
    int videomemPos;
    Registers registers; 
    size_t ip;
    MyStack stack;
};

#endif //!INVOKER_H
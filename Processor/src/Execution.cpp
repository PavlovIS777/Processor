#include "Execution.h"
#include "Types.h"
#include "DSL.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"
#include "Tools.h"

#define PUSH_       PUSH_FUNC(invoker)
#define PUSH_STACK  pushMyStack(&invoker->stack, (ptr_t)&num)
#define POP_        POP_FUNC(invoker)
#define JMP_        JMP_FUNC(invoker)
#define HLT_        if(HLT_FUNC()) return;
#define RET_        RET_FUNC(invoker)
#define CALL_       CALL_FUNC(invoker)
#define IN_         IN_FUNC(invoker)
#define OUT_        OUT_FUNC(invoker)
#define POP_STACK   popMyStackInt(&(invoker->stack))

Invoker invokerCtor()
{
    FILE* invokerInput = fopen("output.bin", "rb");
    assert(invokerInput &&  "WRONG INPUT FILE, FIRST DO ASS");
    fseek(invokerInput, 0, SEEK_END);
    int bytes = ftell(invokerInput);
    rewind(invokerInput);

    Invoker invoker;
    invoker.compiledCode = (c_string)safeCalloc(bytes + 1, sizeof(char));
    fread(invoker.compiledCode, sizeof(char), bytes, invokerInput);

    invoker.codeBytesLen = bytes;
    invoker.videomemPos = 1<<13;
    invoker.registers = {0, 0, 0, 0};
    invoker.memory       = (int*)safeCalloc(MAX_MEM_SIZE, sizeof(int));
    invoker.ip           = 0;
    invoker.stack        = createStack(int);

    return invoker;
}

void invokerDtor(Invoker* invoker)
{
    free(invoker->compiledCode);
    myStackDestructor(&(invoker->stack));
}


#define DEF_CMD(CMD, ID, CODE)  \
        case ID:                \
            CODE                \
            break;              \

#define ZERO_FLAG ((invoker->compiledCode[invoker->ip]) & (1<<7)) >> 7        

void executeProgram(Invoker* invoker)
{
    while(true)
    {
        if (ZERO_FLAG) 
        {
            invoker->compiledCode[invoker->ip] &= 127;
            if (invoker->registers.ex) 
            {
                invoker->ip += 4 + 4*invoker->compiledCode[invoker->ip+3];
                continue;
            }
        }

        switch (invoker->compiledCode[invoker->ip])
        {
            #include "DEF_CMD.h"
            default:
                assert(0 && "UNKNOWN CMD :^(");
                break;
        }
    }
}

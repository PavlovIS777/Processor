#include "Execution.h"
#include "Types.h"
#include "DSL.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"
#include "Tools.h"

#define PUSH_ PUSH_FUNC(invoker)
#define POP_  POP_FUNC(invoker)
#define JMP_  JMP_FUNC(invoker)
#define HLT_  HLT_FUNC(invoker)
#define RET_  RET_FUNC(invoker)
#define CALL_ CALL_FUNC(invoker)

Invoker invokerCtor()
{
    FILE* invokerInput = fopen("output.bin", "rb");
    assert(invokerInput &&  "WRONG INPUT FILE, FIRST DO ASS");
    fseek(invokerInput, 0, SEEK_END);
    size_t bytes = ftell(invokerInput);
    rewind(invokerInput);

    Invoker invoker;
    invoker.compiledCode = (c_string)safeCalloc(bytes + 1, sizeof(char));
    fread(invoker.compiledCode, sizeof(char), bytes, invokerInput);

    invoker.videomemPos = 1<<13;
    invoker.registers = {0, 0, 0, 0};
    invoker.memory       = (int*)safeCalloc(MAX_MEM_SIZE, sizeof(int));
    invoker.ip           = 0;
    invoker.stack        = createStack(sizeof(int));

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

void executeProgram(Invoker* invoker)
{
    switch (invoker->compiledCode[invoker->ip])
    {
    #include "DEF_CMD.h"
    default:
        assert(0 && "UNKNOWN CMD :^(");
        break;
    }
}
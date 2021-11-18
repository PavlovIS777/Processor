#ifndef DSL_H
#define DSL_H

#pragma GCC diagnostic warning "-Wcast-align"
#pragma GCC diagnostic warning "-Wunused-variable"

#include "InvokerCFG.h"


#define regValue   invoker->compiledCode[invoker->ip + 2]? *(int*)((char*)&(invoker->registers) +  \
                   (invoker->compiledCode[invoker->ip + 2] - 1) * sizeof(int)) : 0                \

#define value      invoker->compiledCode[invoker->ip + 3]? *((int*)&(invoker->compiledCode[invoker->ip+4])) : 0 

#define offset     invoker->compiledCode[invoker->ip + 3]? 8 : 4

#define mem        invoker->compiledCode[invoker->ip + 1]? 1 : 0 

#define reg        invoker->compiledCode[invoker->ip + 2]? \
                   (int*)((char*)&(invoker->registers) + ((invoker->compiledCode[invoker->ip + 2] - 1) * sizeof(int))) : nullptr \

#define jmpIP      *(int*)&(invoker->compiledCode[invoker->ip + 1])

void PUSH_FUNC(Invoker* invoker);

int POP_FUNC(Invoker* invoker);

int HLT_FUNC();

void RET_FUNC(Invoker* invoker);

void CALL_FUNC(Invoker* invoker);

void JMP_FUNC(Invoker* invoker);

void IN_FUNC(Invoker* invoker);

void OUT_FUNC(Invoker* invoker);

#endif
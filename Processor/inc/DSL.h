#ifndef DSL_H
#define DSL_H

#include "StackOverflow.h"
#include "InvokerCFG.h"
#include "assert.h"

#define regValue   invoker->compiledCode[invoker->ip + 2]? *(int*)((char*)&(invoker->registers) +  \
                   (invoker->compiledCode[invoker->ip + 2] - 1) * sizeof(int)) : 0                \

#define value      invoker->compiledCode[invoker->ip + 3]? *((int*)&(invoker->compiledCode[invoker->ip+4])) : 0 \

#define mem        invoker->compiledCode[invoker->ip + 1]? 1 : 0 \

#define reg        invoker->compiledCode[invoker->ip + 2]? \
                   (int*)((char*)&(invoker->registers) + ((invoker->compiledCode[invoker->ip + 2] - 1) * sizeof(int))) : nullptr \

int PUSH_FUNC(Invoker* invoker);

int POP_FUNC(Invoker* invoker);

int HLT_FUNC(Invoker* invoker);

int RET_FUNC(Invoker* invoker);

void CALL_FUNC(Invoker* invoker);

void JMP_FUNC(Invoker* invoker);

#endif
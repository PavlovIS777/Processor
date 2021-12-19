#include "DSL.h"
#include "assert.h"
#include "StackOverflow.h"
#include <stdio.h>

void PUSH_FUNC(Invoker* invoker)
{                                                 
    switch (mem)                                                                 
    {                                                                            
    case 1:                                                                      
        invoker->memory[invoker->memPos++] = value + regValue;                     
        break;                                                                   
    case 0:
    {                                                                     
        int num = value + regValue;                                              
        pushMyStack(&invoker->stack, (ptr_t)&num);
        break;
    }                               
    default:                                                                     
        assert(0 && "WRONG PUSH EXECUTE");                                       
        break;                                                                   
    }
    invoker->ip += offset;
}

int POP_FUNC(Invoker* invoker)         
{                          
    switch (invoker->compiledCode[invoker->ip+2])                                                            
    {                                                                            
    case 0:
        invoker->ip += 4;                                                                      
        return popMyStackInt(&(invoker->stack));                                          
        break;                                                                   
    default:                                                                     
        *(reg) = popMyStackInt(&(invoker->stack));   
        invoker->ip += 4;
        return 0;                            
        break;                                                                   
    }                                                                
    }

int HLT_FUNC()
{
    return 1;
}

void RET_FUNC(Invoker* invoker)
{
    invoker->ip = popMyStackInt(&(invoker->stack)) + 8;
}

void CALL_FUNC(Invoker* invoker)
{
    pushMyStack(&(invoker->stack), (ptr_t)&(invoker->ip));
    invoker->ip = jmpIP;
}

void JMP_FUNC(Invoker* invoker)
{
    invoker->ip = jmpIP;
}

void IN_FUNC(Invoker* invoker)
{
    int tmp;
    scanf("%d", &tmp);
    pushMyStack(&(invoker->stack), (ptr_t)&tmp);
    invoker->ip += 4;
}

void OUT_FUNC(Invoker* invoker)
{
    printf("%d", popMyStackInt(&(invoker->stack)));
    invoker->ip += 4;
}

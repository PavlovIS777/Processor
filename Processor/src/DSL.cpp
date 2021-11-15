#include "DSL.h"
#include "assert.h"
#include "StackOverflow.h"
#include <stdio.h>

void PUSH_FUNC(Invoker* invoker, int* numberPtr)
{              
    if (numberPtr) 
    {
        pushMyStack(&invoker->stack, (ptr_t)numberPtr);
        return;
    }                                                   
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
        return popMyStackInt(&(invoker->stack));
        invoker->ip += 4;                                          
        break;                                                                   
    default:                                                                     
        *(reg) = popMyStackInt(&(invoker->stack));   
        invoker->ip += 4;
        return *(reg);                            
        break;                                                                   
    }                                                                
}

int HLT_FUNC(Invoker* invoker)
{
    return 1;
}

int RET_FUNC(Invoker* invoker)
{
    POP_FUNC(invoker);
}

void CALL_FUNC(Invoker* invoker)
{
    PUSH_FUNC(invoker, &(invoker->ip));
    invoker->ip = value;
}

void JMP_FUNC(Invoker* invoker)
{
    invoker->ip = value;
}

void IN_FUNC(Invoker* invoker)
{
    int tmp;
    scanf("%d", &tmp);
    PUSH_FUNC(invoker, &tmp);
    invoker->ip += 4;
}


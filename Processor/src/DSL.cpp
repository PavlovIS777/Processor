#include "DSL.h"
#include "InvokerCFG.h"

int PUSH_FUNC(Invoker* invoker)
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

    return 0;                                                                      
}

int POP_FUNC(Invoker* invoker)         
{                                                        
    switch (regValue)                                                            
    {                                                                            
    case 0:                                                                      
        return popMyStackInt(&(invoker->stack));                                              
        break;                                                                   
    default:                                                                     
        *(reg) = popMyStackInt(&(invoker->stack));   
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
    PUSH_FUNC(invoker);
}

void JMP_FUNC(Invoker* invoker)
{
    PUSH_FUNC(invoker);
}



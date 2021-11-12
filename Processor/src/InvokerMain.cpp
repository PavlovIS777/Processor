#include "Execution.h"
#include "InvokerCFG.h"
#include "StackOverflow.h"
#include "StackConfig.h"
#include <stdio.h>

//extern FILE* LOG;

//extern void stackIntPrint(const MyStack* stack, size_t blockPos, size_t elemPos);
//extern void dumpStack(const MyStack* stack, 
//				   void printElement(const MyStack* stack, size_t blockPos, size_t elemPos));
int main(void)
{
    Invoker invoker = invokerCtor();
    //dumpStack(&(invoker.stack), stackIntPrint);
    executeProgram(&invoker);
    int tmp = 0;
    return 0;
}
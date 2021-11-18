#include "Execution.h"
#include "InvokerCFG.h"
#include "StackOverflow.h"
#include "StackConfig.h"
#include <stdio.h>

int main(void)
{
    Invoker invoker = invokerCtor();
    executeProgram(&invoker);
    return 0;
}
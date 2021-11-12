#ifndef EXECUTION_H
#define EXECUTION_H
#include "Types.h"
#include "InvokerCFG.h"

Invoker invokerCtor();

void executeProgram(Invoker* invoker);

void invokerDtor(Invoker* invoker);

#endif
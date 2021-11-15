#include "StackOverflow.h"
#include "Tools.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <string.h>

#if PROTECTION_LEVEL == 1
const canary_t CANARY = 0xBADCACA;
FILE* LOG = fopen("LOG.TXT", "w+");

size_t hashFunc(const char* str, size_t len, size_t init) {
    size_t hash = init;
    for (size_t it = 0; it < len; str++, it++) {
        hash += (unsigned char)(*str);
        hash += (hash << 20);
        hash ^= (hash >> 12);
    }

    hash += (hash << 6);
    hash ^= (hash >> 22);
    hash += (hash << 29);

    return hash;
}

size_t myHash(const MyStack* stack)
{
    ptr_t startStack = (ptr_t) (&stack->firstCanary);
    ptr_t endStack = (ptr_t)&stack->secondCanary + sizeof(canary_t);

    size_t hash = 0;
    for (int i = 0; i < stack->dataStruct.blockCount; ++i)
    {
        hash = hashFunc(stack->dataStruct.dataBlocks[i]-sizeof(canary_t),
                        STACK_BLOCK_CAPACITY * stack->size + sizeof(canary_t), hash);
    }

    size_t stackHashLen = endStack - startStack;
    size_t hashStack = hashFunc(startStack, stackHashLen, hash);

    return hashStack;
}

void dumpStack(const MyStack* stack, void printElement(const MyStack* stack, size_t blockPos, size_t elemPos))
{
    fprintf(LOG, "\t  INFO ABOUT STACK\n\n"
        "\tLEFT CANARY:  %zu\n"
        "\tHASH:         %zu\n"
        "\tSIZE:         %zu\n"
        "\tLENGHT:       %zu\n"
        "\tCAPACITY:     %zu\n"
        "\tRIGHT CANARY: %zu\n\n"
        "\t\tARRAY\n\n"
        "\tLEFT ARRAY CANARY:  %zu\n"
        "\tRIGHT ARRAY CANARY: %zu\n"
        "\tARRAY PTR: %p\n\n", stack->firstCanary, stack->hash, stack->size,
        stack->len, STACK_BLOCK_CAPACITY * stack->dataStruct.blockCount, stack->secondCanary,
        *(canary_t*)(stack->dataStruct.dataBlocks[0] - sizeof(canary_t)),
        *(canary_t*)(stack->dataStruct.dataBlocks[0] + STACK_BLOCK_CAPACITY * stack->size), stack->dataStruct.dataBlocks[0]);
    size_t len = stack->len;
    fprintf(LOG, "\t  STACK BLOCKS:\n\n");
    for (int j = 0; j < stack->dataStruct.blockCount; ++j)
    {
        fprintf(LOG, "\tARRAY %d\n", j + 1);
        for (int i = 0; (j + 1 == stack->dataStruct.blockCount && i < stack->dataStruct.currentLen) ||
            (j + 1 < stack->dataStruct.blockCount && i < 256); i++)
        {
            fprintf(LOG, "\t************\n"
                         "\tBLOCK: %d\n"
                         "\tBLOCK PTR: %p\n", i + 1, (stack->dataStruct.dataBlocks[j] + i * stack->size));
            printElement((MyStack*)stack, j, i);
            fprintf(LOG, "\t************\n\n");
        }
    }
}

int isValidMyStack(const MyStack* stack)
{
    if (stack == nullptr)
    {
        fprintf(LOG, "\tNULLPTR STACK\n\n");
        dumpStack(stack, stackIntPrint);
        return NULL_PTR_ERROR;
    }
    if (stack->dataStruct.dataBlocks == nullptr)
    {
        fprintf(LOG, "\tNULLPTR ARRAY\n\n");
        dumpStack(stack, stackIntPrint);
        return ARRAY_PTR_ERROR;
    }
    if (stack->firstCanary != CANARY)
    {
        fprintf(LOG, "\tLEFT CANARY ERROR\n\n");
        dumpStack(stack, stackIntPrint);
        return LEFT_CANARY_ERROR;
    }
    if (stack->secondCanary != CANARY)
    {
        fprintf(LOG, "\tRIGHT CANARY ERROR\n\n");
        dumpStack(stack, stackIntPrint);
        return RIGHT_CANARY_ERROR;
    }
    int ERROR = NO_ERROR;
    for (int i = 0; i < stack->dataStruct.blockCount; ++i)
    {
        if (*(canary_t*)(stack->dataStruct.dataBlocks[i] - sizeof(canary_t)) != CANARY)
        {
            ERROR = ARRAY_LEFT_CANARY_ERROR;
            fprintf(LOG, "ARRAY LEFT CANARY ERROR, ARRAY BLOCK: %d\n", i + 1);
        }
    }
    if (ERROR == ARRAY_LEFT_CANARY_ERROR)
    {
        dumpStack(stack, stackIntPrint);
        return ERROR;
    }
    for (int i = 0; i < stack->dataStruct.blockCount; ++i)
    {
        if (*(canary_t*)(stack->dataStruct.dataBlocks[i] + STACK_BLOCK_CAPACITY * stack->size) != CANARY)
        {
            ERROR = ARRAY_RIGHT_CANARY_ERROR;
            fprintf(LOG, "ARRAY RIGHT CANARY ERROR, ARRAY BLOCK: %d\n", i + 1);
        }
    }
    if (ERROR == ARRAY_RIGHT_CANARY_ERROR)
    {
        dumpStack(stack, stackIntPrint);
        return ERROR;
    }
    if (myHash(stack) != stack->hash)
    {
        fprintf(LOG, "\tHASH ERROR\n\n");
        dumpStack(stack, stackIntPrint);
        return HASH_ERROR;
    }
    return NO_ERROR;
}

void stackIntPrint(const MyStack* stack, size_t blockPos, size_t elemPos)
{
    fprintf(LOG, "\tVALUE: %d\n", *(int*)(stack->dataStruct.dataBlocks[blockPos] + 
           elemPos * stack->size));
}

#endif

void newStackDataBlock(MyStack* stack)
{
#if PROTECTION_LEVEL == 1
    
    size_t extraMem = sizeof(canary_t) / stack->size;
    ptr_t newBlock = (ptr_t)safeCalloc(STACK_BLOCK_CAPACITY + 2 * extraMem, stack->size);
    *(canary_t*)newBlock = CANARY;
    *(canary_t*)(newBlock + STACK_BLOCK_CAPACITY * stack->size + sizeof(canary_t)) = CANARY;
    stack->dataStruct.dataBlocks[stack->dataStruct.blockCount] = newBlock + sizeof(canary_t);

#else
    ptr_t newBlock = (ptr_t)safeCalloc(STACK_BLOCK_CAPACITY, stack->size);
    stack->dataStruct.dataBlocks[stack->dataStruct.blockCount] = newBlock;

#endif // PROTECTION_LEVEL == 1
}

MyStack createStack_(size_t size)
{
    ptr_t* dataBlocks = (ptr_t*)safeCalloc(STACK_BLOCK_CAPACITY, sizeof(ptr_t));
    MyData dataStruct = {0, 0, dataBlocks};
#if PROTECTION_LEVEL == 1
    MyStack stack = { CANARY, 0, size, dataStruct, nullptr, CANARY, 0 };
#else
    MyStack stack = { 0, size, dataStruct, nullptr };
#endif
    newStackDataBlock(&stack);
    stack.dataStruct.blockCount++;
#if PROTECTION_LEVEL == 1
    stack.hash = myHash(&stack);
    isValidMyStack(&stack);
#endif
    return stack;
}

void pushMyStack(MyStack* stack, ptr_t element)
{
    ENABLE_PROTECTION(assert(!isValidMyStack(stack));)

    int tmp = 256 * STACK_BLOCK_CAPACITY;
    int tmp2 = stack->len + STACK_BLOCK_CAPACITY * (stack->dataStruct.blockCount - 1) + 1;
    if (stack->len + STACK_BLOCK_CAPACITY * (stack->dataStruct.blockCount - 1) + 1 >=
        256 * STACK_BLOCK_CAPACITY)
    {
        assert(0 && "Stack Overflow");
    }
    if (stack->len == STACK_BLOCK_CAPACITY * stack->dataStruct.blockCount)
    {
        newStackDataBlock(stack);
        stack->dataStruct.blockCount++;
        stack->dataStruct.currentLen = 0;
    }
    stack->top = stack->dataStruct.dataBlocks[stack->dataStruct.blockCount - 1] + stack->dataStruct.currentLen * stack->size;
    memcpy(stack->top, element, stack->size);
    stack->len++;
    stack->dataStruct.currentLen++;

    ENABLE_PROTECTION(stack->hash = myHash(stack);)
}

ptr_t popMyStack(MyStack* stack)
{
    ENABLE_PROTECTION(assert(!isValidMyStack(stack));)

    if (stack->len == 0) { return nullptr; }

    ptr_t top = (ptr_t)safeCalloc(1, stack->size);
    memcpy(top, stack->top, stack->size);

    if (stack->dataStruct.currentLen - 1 == 0 && stack->dataStruct.blockCount > 1) 
    {
        free(stack->dataStruct.dataBlocks[stack->dataStruct.blockCount - 1]);
        stack->dataStruct.currentLen = STACK_BLOCK_CAPACITY - 1;
        stack->dataStruct.blockCount--;
    }
    else
    {
        stack->dataStruct.currentLen--;
    }
    stack->top -= stack->size; 
    stack->len--;
  
    ENABLE_PROTECTION(stack->hash = myHash(stack);)

    return top;
}

int popMyStackInt(MyStack* stack)
{
    ENABLE_PROTECTION(assert(!isValidMyStack(stack));)

    if (stack->len == 0) {assert(0 && "STACK IS EMPTY!");}

    int popedElem = *(int*)(stack->top);
    
    if (stack->dataStruct.currentLen - 1 == 0 && stack->dataStruct.blockCount > 1) 
    {
        free(stack->dataStruct.dataBlocks[stack->dataStruct.blockCount - 1]);
        stack->dataStruct.currentLen = STACK_BLOCK_CAPACITY - 1;
        stack->dataStruct.blockCount--;
    }
    else
    {
        stack->dataStruct.currentLen--;
    }
    stack->top -= stack->size; 
    stack->len--;
  
    ENABLE_PROTECTION(stack->hash = myHash(stack);)

    return popedElem;
}


void myStackDestructor(MyStack* stack)
{
    ENABLE_PROTECTION(assert(!isValidMyStack(stack));)
    for (size_t i = 0; i < stack->dataStruct.blockCount; ++i)
    {
        memset(stack->dataStruct.dataBlocks[i], 0, STACK_BLOCK_CAPACITY);
        free(stack->dataStruct.dataBlocks[i]);
    }

    memset(stack->dataStruct.dataBlocks, 0, 256);
    free(stack->dataStruct.dataBlocks);

    memset(stack, 0, sizeof(MyStack));
}
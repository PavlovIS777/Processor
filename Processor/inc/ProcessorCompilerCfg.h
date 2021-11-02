#ifndef PROCESSORCOMPILERCFG_H
#define PROCESSORCOMPILERCFG_H

#include <stdint.h>

#define DEF_CMD(CMD, ID)       \
    CMD_##CMD = ID,            \

enum cmdId
{
    ERROR_CMD = -1,
    #include <DEF_CMD.h>
    CMD_LABEL,
    CMD_ENUM_LEN
};  

#undef DEF_CMD

#define DEF_REG(NAME, ID)   \
    REG_##NAME = ID - 1,    \

enum regId
{
    REG_EMPTY = -1,
    #include "DEF_REG.h"
    REG_ENUM_LEN
};

#undef DEF_REG

struct label
{
    size_t hash;
    int ip;
};

struct Marks
{
    int marksCount;
    int capacity;
    label* labels;
};

struct Labels
{
    int labelsCount;
    int capacity;
    label* labels;
};

struct CMD
{
    c_string cmd;    
    uint8_t cmdId;
    uint8_t mem;
    uint8_t reg;
    uint8_t scannedNum;
    int num;
};

#endif //!PROCESSORCOMPILERCFG_H
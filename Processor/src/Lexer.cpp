#include "Lexer.h"
#include "assert.h"
#include "ProcessorCompilerCfg.h"
#include "Assembling.h"
#include <unistd.h>
#include <stdio.h>


uint8_t findBrackets (c_string* args)
{
    skipSpaces(args);
    if (strchr(*args, '[') == nullptr && strchr(*args, ']') == nullptr)
        return 0;
    skipSpaces(args);

    int bracketsCount = 0;
    int bracketsSeq = 0;
    int len = strlen(*args);

    for (int i = 0; i < len && bracketsSeq >= 0 && bracketsCount <= 2; ++i)
    {
        if ((*args)[i] == '['? ++bracketsCount : 0)
            ++bracketsSeq;
        else if ((*args)[i] == ']'? ++bracketsCount : 0)
            --bracketsSeq;
    }
    if (bracketsSeq != 0 || bracketsCount != 2)
        assert(0 && "BRACKETS ERROR");

    *args = strchr(*args, '[') + 1;
    return 1;
}

size_t makeHash(const char* str, size_t len) 
{
    size_t hash = 0;
    for (size_t it = 0; it < len; str++, it++) 
    {
        hash += (unsigned char)(*str);
        hash += (hash << 20);
        hash ^= (hash >> 12);
    }

    hash += (hash << 6);
    hash ^= (hash >> 22);
    hash += (hash << 29);

    return hash;
}

int skipSpaces (c_string* args)
{
    int spaces = 0;
    while (**args == ' '? ++(*args) : 0)
        spaces++;
    return spaces;
}


#define DEF_REG(NAME, ID) \
    case ID:              \
        *args += 2;       \
        return ID;        \
        break;            \

uint8_t findRegister (c_string* args)
{
    skipSpaces(args);

    uint8_t regId = getRegId (*args);
    switch (regId)
    {
        #include "DEF_REG.h"
    }
    return 0;
}

#undef DEF_REG



int8_t findSign(c_string* args)
{
    skipSpaces(args);
    if (**args == '+')
    {
        (*args)++;
        return 1;
    }
    else if (**args == '-')
    {
        (*args)++;
        return -1;
    }
    else return 0;
}


uint8_t findNum(c_string* args, int* num)
{
    int8_t sign = findSign(args);
    uint8_t scanned = 0;
    scanned = sscanf(*args, "%d", num);
    scanned %= 255;
    if (scanned)
        while (**args && **args >= '0' && **args <= '9') {++*args;}
    if(sign) {*num *= sign;}
    return scanned;
}

#define DEF_CMD(CMD, ID)                \
        if (hash == CMD_HASHES[ID])     \
            return ID;                  \

int8_t getCMDId (c_string* lexem)
{
    skipSpaces(lexem);
    if (*(*lexem + strlen(*lexem) - 1) == ':') {return CMD_LABEL;}
    uint64_t tmp = CMD_HASHES[0];
    c_string offset = strchr(*lexem, ' ')? strchr(*lexem, ' ') : *lexem + strlen(*lexem);
    uint64_t hash = makeHash(*lexem, offset - *lexem);
    *lexem = offset;
    #include "DEF_CMD.h"

    return ERROR_CMD;
}

#undef DEF_CMD

#define DEF_REG(CMD, ID)                \
        if (hash == REG_HASHES[ID])     \
        {                               \
            *lexem += 2;                \
            return ID;                  \
        }                               \

uint8_t getRegId (c_string lexem)
{
    uint64_t hash = makeHash(lexem, 2);
    #include "DEF_REG.h"

    return REG_EMPTY;
}

#undef DEF_REG

void makeLexem(CMD* cmd)
{
    cmd->mem = findBrackets(&(cmd->cmd));
    cmd->reg = findRegister(&(cmd->cmd));
    cmd->scannedNum = findNum(&(cmd->cmd), &(cmd->num));
    if(cmd->mem) {cmd->cmd++;}
}


void makeMark(int* ip, c_string compiledStr, c_string cmd, Marks* marks, char cmdId)
{
    skipSpaces(&cmd);
    int labelLen = strlen(cmd);
    marks->labels[marks->marksCount++] = {makeHash(cmd, labelLen), (*ip)};
    compiledStr[(*ip)++] = cmdId;
    int defaultValue = -1;
    memcpy(&compiledStr[*ip], &defaultValue, sizeof(int));
    (*ip) += 4;
}   

void makeJMP (c_string compiledStr, Marks* marks, Labels* labels)
{
    for (int i = 0; i < marks->marksCount; ++i)
    {
        for (int j = 0; j < labels->labelsCount; ++j)
        {
            if (marks->labels[i].hash == labels->labels[j].hash)
            {
                memcpy(&compiledStr[marks->labels[i].ip + 1], &labels->labels[j].ip, 4);
                break;
            }

            if (j + 1 == labels->labelsCount) {assert(0 && "LABEL NOT FOUND");}
        }
    }
}

void resetCmdStruct(CMD* cmd)
{
    cmd->mem = 0;
    cmd->num = 0;
    cmd->reg = 0;
    cmd->cmdId = -1;
}

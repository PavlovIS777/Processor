#include "Lexer.h"

int findBrackets (c_string* args)
{
    skipSpaces(args);
    if (strchr(*args, '[') == nullptr && strchr(*args, ']') == nullptr)
        return 0;
    skipSpaces(args);
    int bracketsCount = 0;
    int len = strlen(*args);

    for (int i = 0; i < len && bracketsCount >= 0 && bracketsCount < 2; ++i)
    {
        if ((*args)[i] == '[')
            ++bracketsCount;
        else if ((*args)[i] == ']')
            --bracketsCount;
    }

    if (bracketsCount < 0 || bracketsCount >= 1)
        return -1;
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


int findRegister (c_string* args)
{
    skipSpaces(args);
    int regHash = makeHash(*args, 2);

    switch (regHash)
    {
    case ax:
        *args += 2;
        return 1;
    case bx:
        *args += 2;
        return 2;
    case cx:
        *args += 2;
        return 3;
    case dx:
        *args += 2;
        return 4;
    default:
        return 0;
    }
    return 0;
}


int findSign(c_string* args)
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
        }
    }
}



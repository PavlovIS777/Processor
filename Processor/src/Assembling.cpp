#include "Assembling.h"
#include "ProcessorCompilerCfg.h"
#include "Lexer.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

c_string mkInputDir (c_string filename)
{
    c_string dir = (c_string)safeCalloc(20, sizeof(char));
    strcpy(dir, "src/");
    strcat(dir, filename);
    return dir;
}

void* safeCalloc(size_t count, size_t size)
{
    void* temp = calloc(count, size);
    if (temp != nullptr)
        return temp;
    else
    {
        printf("Can't allocte memory.");
        assert(temp);
        return nullptr;
    }
}

c_string strParser(c_string string, int* countStr)
{
    c_string rawCode = (c_string)safeCalloc(strlen(string) + 10, sizeof(char));
    int len = strlen(string);
    int currentLen = 0;
    for (int i = 0, j = 0; i < len; ++i)
    {
        if (string[i] == '#')
        {
            if (currentLen == 0)
                while(i < len? string[++i] != '\n' : 0);
            else
            {
                while(i < len? string[++i] != '\n' : 0);
                rawCode[j++] = '\0';
                currentLen = 0;
                ++(*countStr);
            }
            continue;
        }

        if (string[i] == '\n')
        {
            rawCode[j++] = '\0';
            currentLen = 0;
            ++(*countStr);
        }
        else
        {
            rawCode[j++] = string[i];
            ++currentLen;
        }
    }
    if (string[len] != '\n' && currentLen != 0)
        ++(*countStr);
    return rawCode;
}

c_string doAssembler(c_string dir)
{
    FILE* assemblerInput = fopen(dir, "rb");

    if (assemblerInput == nullptr)
    {
        assemblerInput = fopen("input.bin", "rb");
        // printf("Can't open input file");
        // assert(assemblerInput);
    }

    fseek(assemblerInput, 0, SEEK_END);
    size_t bytes = ftell(assemblerInput);
    rewind(assemblerInput);

    char* assemblerStr = (char*)safeCalloc(bytes + 1, sizeof(char));

    fread(assemblerStr, sizeof(char), bytes, assemblerInput);
    
    int strCount = 0;
    c_string rawCodeAsm = strParser(assemblerStr, &strCount);

    c_string compiledFile = compiler(rawCodeAsm, strCount);
    
    return compiledFile;
}

c_string compiler (c_string rawCode, int cmdCount)
{
    int rawLen = strlen(rawCode);
    c_string compiledStr = (c_string)safeCalloc(rawLen, sizeof(char));
    int ip = 0; //instraction pointer
    c_string cmd = nullptr;

    Labels labels = {0, 100, (label*)safeCalloc(100, sizeof(label))};
    Marks marks = {0, 100, (label*)safeCalloc(100, sizeof(label))};
    while (cmdCount--)
    {
        if (ip + 10 > rawLen)
        {
            rawLen *= 2;
            compiledStr = (c_string)realloc(compiledStr, rawLen);
            assert(compiledStr);
        }

        cmd = strchr(rawCode, ' ');
        size_t hashCmd = 0;
        int cmdIdLen = -1;
        if (cmd != nullptr)
        {
            cmdIdLen = cmd - rawCode;
            hashCmd = makeHash(rawCode, cmdIdLen);
        }

        switch (hashCmd)
        {
        case PUSH:
            {
            int num = 0;
            int bracket = findBrackets(&cmd);
            int reg = findRegister(&cmd);
            int sign = findSign(&cmd);
            //printf("reg: %d\nsign: %d\nbrack: %d\n", reg, sign, bracket);
            
            skipSpaces(&cmd);
            int scanned = sscanf(cmd, "%d", &num) + 1;
            if (!scanned)
            {
                if (sign)
                    abort();
                if (!reg)
                    abort();
            }
            num = bracket? num*sign : num;

            compiledStr[ip++] = 1;
            compiledStr[ip++] = reg;
            compiledStr[ip++] = bracket;
            memcpy(&compiledStr[ip], &num, sizeof(num));
            ip += 4;
            break;
            }
        case POP:
            {
                int num = 0;
                int bracket = findBrackets(&cmd);
                int reg = findRegister(&cmd);
                int sign = findSign(&cmd);

                skipSpaces(&cmd);
                int scanned = sscanf(cmd, "%d", &num) + 1;

                if (!reg && scanned)    {abort();}
                if (reg && scanned && !bracket) {abort();}

                compiledStr[ip++] = 2;
                compiledStr[ip++] = reg;
                compiledStr[ip++] = bracket;
                memcpy(&compiledStr[ip], &num, sizeof(num));
                ip += 4;
                break;
            }
        case ADD:
            {
                compiledStr[ip++] = 3;
            }
        case SUB:
            {
                compiledStr[ip++] = 4;
            }
        case MUL:
            {
                compiledStr[ip++] = 5;
            }
        case JMP:
            {
                skipSpaces(&cmd);
                marks.labels[marks.marksCount++] = {makeHash(cmd, cmd - strchr(cmd, ' ')), ip};
            }
        default:
            {
            c_string labelStr;
            if ((labelStr = strtok(rawCode, ":")) != nullptr)
            {
                labels.labels[labels.labelsCount++] = {makeHash(labelStr, strlen(labelStr)), ip};
            }
            else
            {
                assert(0 && "Wrong label");
            }
            break;
            }
        }

        rawCode += strlen(rawCode) + 1;
    }

    FILE* compiledFile = fopen("output.bin", "wb+");
    fwrite(compiledStr, sizeof(char), ip, compiledFile);
    printf("%s", compiledStr);
    return compiledStr;
}
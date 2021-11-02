#include "Assembling.h"
#include "Lexer.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

extern uint64_t CMD_HASHES[CMD_ENUM_LEN];
extern uint64_t REG_HASHES[REG_ENUM_LEN];


c_string mkInputDir (c_string filename)
{
    c_string dir = (c_string)safeCalloc(20, sizeof(char));
    strcpy(dir, "src/");
    strcat(dir, filename);
    return dir;
}

#define DEF_CMD(CMD, ID)                                    \
        CMD_HASHES[ID] = makeHash(#CMD, strlen(#CMD));      \
        printf("%d: %zu\n", ID, CMD_HASHES[ID]);                      \

#define DEF_REG(NAME, ID)                                   \
        REG_HASHES[ID] = makeHash(#NAME, strlen(#NAME));    \

void init_hashes(uint64_t* CMD_HASHES, uint64_t* REG_HASHES)
{
    #include "DEF_CMD.h"
    #include "DEF_REG.h"
}

#undef DEF_CMD
#undef DEF_REG


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
    
    for (int i = 0, j = 0; i < len;)
    {
        if (string[i] == '#')
        {
            if (currentLen == 0)
                while(string[i] != '\n' && i < len) {++i;}
            else
            {
                while(i < len && string[i++] != '\n');
                rawCode[j++] = '\0';
                currentLen = 0;
                ++(*countStr);
            }
        }
        else if (string[i] == '\n')
        {
            if (currentLen == 0)
                while(i < len && string[++i] == '\n');
            else
            {
            rawCode[j++] = '\0';
            currentLen = 0;
            ++(*countStr);
            ++i;
            }
        }
        else if (currentLen == 0 && string[i] == ' ')
            ++i;
        else
        {
            rawCode[j++] = string[i];
            ++currentLen;
            ++i;
        }
    }
    if (string[len] != '\n' && currentLen != 0)
        ++(*countStr);
    return rawCode;
}

void makeCompile(const c_string dir)
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

    char* inputStr = (char*)safeCalloc(bytes + 1, sizeof(char));

    fread(inputStr, sizeof(char), bytes, assemblerInput);
    
    int strCount = 0;
    c_string rawCodeAsm = strParser(inputStr, &strCount);

    makeASSembler(rawCodeAsm, strCount);
}

void outputCompiledProgramm(const c_string compiledStr, int ip)
{
    FILE* compiledFile = fopen("output.bin", "wb+");
    fwrite(compiledStr, sizeof(char), ip, compiledFile);
}

#define DEF_CMD(CMD, ID)                                                                        \
        case ID:                                                                                \
            {                                                                                   \
            cmdStruct.cmdId = ID;                                                               \
            makeLexem(&cmdStruct);                                                              \
            if (cmdStruct.scannedNum)                                                           \
            {                                                                                   \
                memcpy(&compiledStr[ip], &cmdStruct.cmdId, 8);                                  \
                ip += 8;                                                                        \
            }                                                                                   \
            else                                                                                \
            {                                                                                   \
                memcpy(&compiledStr[ip], &cmdStruct.cmdId, 4);                                  \
                ip += 4;                                                                        \
            }                                                                                   \
            skipSpaces(&cmdStruct.cmd);                                                         \
            if(*cmdStruct.cmd != '\0') {assert(0 && "WRONG COMMAND, CHECK LEXEM ENDING.");}     \
            cmdStruct.cmd += 1;                                                                 \
            resetCmdStruct(&cmdStruct);                                                         \
            }                                                                                   \
            break;                                                                              \


void makeASSembler(c_string rawCode, int cmdCount)
{
    int rawLen = strlen(rawCode);
    c_string compiledStr = (c_string)safeCalloc(rawLen, sizeof(char));
    int ip = 0; //instraction pointer

    Labels labels = {0, 100, (label*)safeCalloc(100, sizeof(label))};
    Marks marks = {0, 100, (label*)safeCalloc(100, sizeof(label))};
    CMD cmdStruct = {rawCode, 0, 0, 0, 0, 0};
    init_hashes(CMD_HASHES, REG_HASHES);

    while (cmdCount--)
    {
        if (ip + 10 > rawLen)
        {
            rawLen *= 2;
            compiledStr = (c_string)realloc(compiledStr, rawLen);
            assert(compiledStr);
        }

        uint8_t cmdId = getCMDId(&(cmdStruct.cmd));

        if (cmdId >= CMD_JMP && cmdId <= CMD_CALL)
        {
            skipSpaces(&cmdStruct.cmd);
            marks.labels[marks.marksCount++] = {makeHash(cmdStruct.cmd, strlen(cmdStruct.cmd)), ip};
            cmdStruct.cmd += strlen(cmdStruct.cmd) + 1;
            compiledStr[ip++] = cmdId;
            ip += 4;
            resetCmdStruct(&cmdStruct);
            continue;
        }
        switch (cmdId)
        {
            case CMD_LABEL:
                labels.labels[labels.labelsCount++] = {makeHash(cmdStruct.cmd, strlen(cmdStruct.cmd) - 1), ip};
                cmdStruct.cmd += strlen(cmdStruct.cmd) + 1;
                resetCmdStruct(&cmdStruct);
                break;
            #include "DEF_CMD.h"
        
            default:
                assert(0 && "COMPILER FAULT.");
                break;
        }
    }

#undef DEF_CMD

    makeJMP(compiledStr, &marks, &labels);
    
    outputCompiledProgramm(compiledStr, ip);
}






#include "Assembling.h"
#include "Lexer.h"
#include "Tools.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

extern uint64_t CMD_HASHES[CMD_ENUM_LEN];
extern uint64_t REG_HASHES[REG_ENUM_LEN];
extern uint8_t COMPILER_SKIP_ZERO_FLAG;

c_string mkInputDir (c_string filename)
{
    c_string dir = (c_string)safeCalloc(20, sizeof(char));
    strcpy(dir, "programs/");
    strcat(dir, filename);
    return dir;
}

#define DEF_CMD(CMD, ID, CODE)                              \
        CMD_HASHES[ID] = makeHash(#CMD, strlen(#CMD));      \

#define DEF_REG(NAME, ID)                                   \
        REG_HASHES[ID] = makeHash(#NAME, strlen(#NAME));    \

void init_hashes()
{
    #include "DEF_CMD.h"
    #include "DEF_REG.h"
}

#undef DEF_CMD
#undef DEF_REG

c_string strParser(c_string string, int* countStr)
{
    c_string rawCode = (c_string)safeCalloc(strlen(string) + 10, sizeof(char));
    size_t len = strlen(string);
    size_t currentLen = 0;
    
    for (size_t i = 0, j = 0; i < len;)
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
        assemblerInput = fopen("programs/input.bin", "rb");
    }
    assert(assemblerInput && "CAN'T OPEN INPUT FILE");
    fseek(assemblerInput, 0, SEEK_END);
    size_t bytes = ftell(assemblerInput);
    rewind(assemblerInput);

    char* inputStr = (char*)safeCalloc(bytes + 1, sizeof(char));

    fread(inputStr, sizeof(char), bytes, assemblerInput);
    
    int strCount = 0;
    c_string rawCodeAsm = strParser(inputStr, &strCount);

    makeASSembler(rawCodeAsm, strCount);
}

void outputCompiledProgram(const c_string compiledStr, size_t ip)
{
    FILE* compiledFile = fopen("output.bin", "wb+");
    fwrite(compiledStr, sizeof(char), ip, compiledFile);
}

#define DEF_CMD(CMD, ID, CODE)                                                                  \
        case ID:                                                                                \
            {                                                                                   \
            cmdStruct.cmdId = ID;                                                               \
            cmdStruct.cmdId |= (COMPILER_SKIP_ZERO_FLAG<<7);                                    \
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
    size_t rawLen = strlen(rawCode);
    c_string compiledStr = (c_string)safeCalloc(rawLen, sizeof(char));
    size_t ip = 0; //instraction pointer

    Labels labels = {0, 100, (label*)safeCalloc(100, sizeof(label))};
    Marks marks = {0, 100, (label*)safeCalloc(100, sizeof(label))};
    CMD cmdStruct = {rawCode, 0, 0, 0, 0, 0};
    init_hashes();

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
                assert(0 && cmdId && "COMPILER FAULT.");
                break;
        }
    }

#undef DEF_CMD

    makeJMP(compiledStr, &marks, &labels);
    
    outputCompiledProgram(compiledStr, ip);
}






#ifndef LEXER_H
#define LEXER_H

#include "Types.h"
#include "ProcessorCompilerCfg.h" 
#include <stdint.h>

uint8_t findBrackets (c_string* args);

size_t makeHash(const char* str, size_t len);

int skipSpaces (c_string* args);

uint8_t findRegister (c_string* args);

int8_t findSign(c_string* args);

uint8_t findNum(c_string* args, int* num);

void makeLexem(CMD* cmd);

int8_t getCMDId (c_string* lexem);

uint8_t getRegId (c_string lexem);

void makeMark(int* ip, c_string compiledStr, c_string cmd, Marks* marks, char cmdId);

void makeJMP (c_string compiledStr, Marks* marks, Labels* labels);

void resetCmdStruct(CMD* cmd);
#endif //!LEXER_H
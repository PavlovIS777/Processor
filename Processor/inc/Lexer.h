#ifndef LEXER_H
#define LEXER_H

#include "Types.h"
#include "ProcessorCompilerCfg.h"

int findBrackets (c_string* args);

size_t makeHash(const char* str, size_t len);

int skipSpaces (c_string* args);

int findRegister (c_string* args);

int findSign(c_string* args);

void makeMark(int* ip, c_string compiledStr, c_string cmd, Marks* marks, char cmdId);

void makeJMP (c_string compiledStr, Marks* marks, Labels* labels);
#endif //!LEXER_H
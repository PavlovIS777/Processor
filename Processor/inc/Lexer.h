#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

int findBrackets (c_string* args);

size_t makeHash(const char* str, size_t len);

void skipSpaces (c_string* args);

int findRegister (c_string* args);

int findSign(c_string* args);

#endif //!LEXER_H
#include "Lexer.h"

int findBrackets (c_string args)
{
    if (strchr(args, '[') == nullptr && strchr(args, ']') == nullptr)
        return 0;

    int bracketsCount = 0;
    while (*args && bracketsCount >= 0 && bracketsCount < 2)
    {
        if (*args == '#')
            break;
        else if (*args == '[')
            ++bracketsCount;
        else if (*args == ']')
            --bracketsCount;
    }

    if (bracketsCount < 0 || bracketsCount >= 2)
        return -1;
    return 1;
}

// c_string findSign(c_string args, int* sign)
// {
//     c_string commentPtr = 
// }
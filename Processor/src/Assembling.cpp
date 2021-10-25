#include "Assembling.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

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

c_string strParser(c_string string, size_t* countStr)
{
    char* symbol    = 0;
    c_string rawCode = (c_string)safeCalloc(strlen(string), sizeof(char));
    int len = 0;
    while (*string)
    {
        if (*string == '#')
        {
            while(*string != '\n'? ++string : 0);
            *rawCode++ = '#';
            ++len;
        }
        if (*string == '\n')
        {
        *rawCode++ = '\0';
        ++len;
        ++(*countStr);
        ++string;
        }

        *rawCode++ = *string++;
        ++len;
    }

    return rawCode - len;
}

c_string doAssembler(int* stringsCount, c_string dir)
{
    FILE* assemblerInput = fopen(dir, "rb");

    if (assemblerInput == nullptr)
    {
        printf("Can't open input file");
        assert(assemblerInput);
    }

    fseek(assemblerInput, 0, SEEK_END);
    size_t bytes = ftell(assemblerInput);
    rewind(assemblerInput);

    char* assemblerStr = (char*)safeCalloc(bytes + 1, sizeof(char));

    fread(assemblerStr, sizeof(char), bytes, assemblerInput);
    
    size_t strCount;
    c_string rawCodeAsm = strParser(assemblerStr, &strCount);



    return rawCodeAsm;
}

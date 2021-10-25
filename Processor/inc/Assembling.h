#ifndef ASSEMBLING_H
#define ASSEMBLING_H

#include "Types.h"  

#define MAXPROCESSORLEN 32768

c_string doAssembler(int* stringsCount, c_string dir);

c_string strParser(c_string string, size_t* countStr);

void* safeCalloc(size_t count, size_t size);

size_t makeHash(const char* str, size_t len);

c_string mkInputDir (c_string filename);
#endif // !ASSEMBLING_H

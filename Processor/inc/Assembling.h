#ifndef ASSEMBLING_H
#define ASSEMBLING_H

#include "Types.h"  

c_string doAssembler(c_string dir);

c_string strParser(c_string string, int* countStr);

void* safeCalloc(size_t count, size_t size);

size_t makeHash(const char* str, size_t len);

c_string mkInputDir (c_string filename);

c_string compiler (c_string rawCode, int cmdCount);

#endif // !ASSEMBLING_H

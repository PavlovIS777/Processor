#ifndef ASSEMBLING_H
#define ASSEMBLING_H

#include "Types.h"
#include <stdint.h>
#include "ProcessorCompilerCfg.h"

uint64_t CMD_HASHES[CMD_ENUM_LEN];
uint64_t REG_HASHES[REG_ENUM_LEN];

void makeCompile(const c_string dir);

c_string strParser(c_string string, int* countStr);

void* safeCalloc(size_t count, size_t size);

size_t makeHash(const char* str, size_t len);

c_string mkInputDir (c_string filename);

void makeASSembler(c_string rawCode, int cmdCount);

void init_hashes(uint64_t* CMD_HASHES, uint64_t* REG_HASHES);

void outputCompiledProgramm(const c_string compiledStr, int ip);
#endif // !ASSEMBLING_H

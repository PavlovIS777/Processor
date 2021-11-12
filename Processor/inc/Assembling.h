#ifndef ASSEMBLING_H
#define ASSEMBLING_H

#include "Types.h"
#include <stdint.h>
#include "ProcessorCompilerCfg.h"

void makeCompile(const c_string dir);

c_string strParser(c_string string, int* countStr);

c_string mkInputDir (c_string filename);

void makeASSembler(c_string rawCode, int cmdCount);

void init_hashes();

void outputCompiledProgram(const c_string compiledStr, size_t ip);
#endif // !ASSEMBLING_H

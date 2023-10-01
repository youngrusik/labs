#ifndef __MACHINE_WORD__
#define __MACHINE_WORD__

#include <stdio.h>
#include <stdint.h>

typedef uint64_t machineWord;

void readMachWord(machineWord* data);
void writeMachWord(machineWord* data);

#endif //__MACHINE_WORD__
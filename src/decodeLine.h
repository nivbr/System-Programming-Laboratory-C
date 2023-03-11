#ifndef DECODELINE    /*dont double define this header*/
#define DECODELINE 

#include "symbolChart.h"

void decode(char line[LINE_LENGTH], symbolChart * chart, int* L, int codeMem[MEMORY_SIZE]);

#endif
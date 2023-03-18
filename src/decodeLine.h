#ifndef DECODELINE    /*dont double define this header*/
#define DECODELINE 

#include "util.h"

void decode(char line[LINE_LENGTH],bool startWLable, symbolChart * chart, int *L, int codeMem[MEMORY_SIZE]);

typedef struct WordShita1{
    unsigned int ERA:2;
    unsigned int rest:12;
}WordShita1;

#endif
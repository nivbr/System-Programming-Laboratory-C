#ifndef PASS    /*dont double define this header*/
#define PASS 

#include "util.h"
#include <ctype.h>

#define FIRST_MEM_CELL 100
#define OP_COUNT 16 /*amount of ops*/
#define OP_MAX_LENGTH 5 /*length of longest op name +'\0'*/

bool pass1(char* filename ,symbolChart * chart, int dataMem[256], int codeMem[MEMORY_SIZE]); /*return true if no errors !*/

#endif
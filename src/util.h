#ifndef UTIL    /*dont double define this header*/
#define UTIL 

#include "symbolChart.h"
#include <ctype.h>

Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]);
void doLine(char* cur_line,int* IC, int* DC, symbolChart * chart, int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE], bool * errorFlag); /*return true if OK, false if error found*/
void clearString(char * s);
bool stringIsEmpty(char* s);
void printMemPic(int mem[MEMORY_SIZE], const char* headline);
void clearStringData(char *s);

#endif
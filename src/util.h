#ifndef UTIL    /*dont double define this header*/
#define UTIL 

#include "symbolChart.h"
#include <ctype.h>

#define FIRST_MEM_CELL 100
#define OP_COUNT 16 /*amount of ops*/
#define OP_MAX_LENGTH 5 /*length of longest op name +'\0'*/

Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]);
void clearString(char * s);
bool stringIsEmpty(char* s);
void printMemPic(int mem[MEMORY_SIZE], const char* headline);
void clearStringData(char *s);
int InitDecodeOp(char line[LINE_LENGTH], int* L);
int token2val(char* token, bool* isReg, bool* isLab, bool* isIm, bool* isErr);
int token2op(char token[LINE_LENGTH]);
int calcL(char line[LINE_LENGTH], bool startWLable);
bool stringIsEmpty(char* s);

#endif
#ifndef UTIL    /*dont double define this header*/
#define UTIL 

#define IMMIDIATE 0
#define LABLE 1
#define REGISTER 3

#include "symbolChart.h"
#include <ctype.h>

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
int opWord2int(OpWord* word);
bool isStringCont(char* s, int from); /*checks if the given string continues from index 'from' (included)*/

#endif
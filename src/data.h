#ifndef datafile
#define datafile

#include <stdio.h>

#define LINE_LENGTH 81          /*maximum lengt of line*/
#define MEMORY_SIZE 256        /*size of memory*/
#define BITS_IN_WORD 14   
#define NUM_OF_REGS 8           /*r0 upto r7 registers*/

typedef enum { false, true } bool;

enum fileTypes{original,afterMacro, entries, externals};

enum attributes{external,entry,code,data};

typedef struct OpWord{
    unsigned int ERA:2;
    unsigned int src:2;
    unsigned int dst:2;
    unsigned int opcode:4;
    unsigned int par1:2;
    unsigned int par2:2;
} OpWord;


#endif
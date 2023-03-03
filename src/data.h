#ifndef datafile
#define datafile

#include <stdio.h>

#define LINE_LENGTH 81          /*maximum lengt of line*/
#define MEMORY_SIZE 256        /*size of memory*/
#define BITS_IN_WORD 14   

typedef enum { false, true } bool;

enum fileTypes{original,afterMacro, entries, externals};

enum attributes{external,entry,code,data};


#endif
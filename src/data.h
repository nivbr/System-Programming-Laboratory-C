#ifndef datafile
#define datafile

#include <stdio.h>

#define LINE_LENGTH 81          /*maximum lengt of line*/
#define MEMORY_SIZE 8192        /*size of memory*/
#define FIELDS_IN_WORD 5        /*5 fields in words |(A4-B3-C1-D3-E7)| == 5  */
#define BITS_IN_FIELD 4         /*every 4 bits transfers to one hexa in the special base */
#define BITS_IN_WORD FIELDS_IN_WORD*BITS_IN_FIELD   /*simply..*/

typedef enum { false, true } bool;

enum fileTypes{original,afterMacro, entries, externals};

enum attributes{external,entry,code,data};


#endif
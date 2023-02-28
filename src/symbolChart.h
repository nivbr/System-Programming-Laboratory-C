#include "data.h"

#ifndef CHARTFILE
#define CHARTFILE

#include <string.h>
#include <stdlib.h>


#define INIT_LINES_CNT 20


typedef struct Line{ /*Row in the symbols table, holds table's cells*/
    char symbol[LINE_LENGTH];   /*symbol LABLE*/
    int value;
    int baseAdrs;       /*baseAdress == -1 -> unkown offset to be filled in parse2*/
    int offset;     
    bool attributes[4]; /*using "enum attributes{external,entry,code,data};" 
                          acts like an on\off state ,checkbox for relevant attributes for current symbol */
}Line;

typedef struct symbolChart{
    Line** linesArray;   /*dynamic 2d array*/
    int size;
    int maxSize;        /*helps me stay in array boundries OR resize it with realloc() */
} symbolChart;

/*public functions*/
void newSymbolChart(symbolChart* chart);    /*constructor*/
void deleteSymbolChart(symbolChart* chart); /*distructor*/
void insertSymbol(Line* line, symbolChart* chart); /*insert new row to chart*/
void printSymbolChart(symbolChart* chart);  /*print full chart for debbuging*/
Line* searchSymbol(symbolChart* chart, char* symbol);   /*search in chart by symbol name (1st column) */
Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]);
void freeSymbol(Line * line);

#endif
#include "symbolChart.h"
#define WORD_FORMAT " %80s"


Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]);  
bool pass1(char* filename ,symbolChart * chartptr);
void copyArr(bool from[4], bool to[4]);     /*obviuos*/

bool pass(char** filesList[4],int listCounters[4]){
    symbolChart * chart;
    int i;
    for (i=0;i<listCounters[afterMacro];i++){
        printf("do [%s] file \n",filesList[afterMacro][i]);
        chart = newSymbolChart();
        pass1(filesList[afterMacro][i],chart);
        printSymbolChart(chart);
        deleteSymbolChart(chart);
    }
    return true;
}

bool pass1(char* filename ,symbolChart * chart){
    Line * line = NULL;
    bool atr[4] = {false,false,false,false};
    int lineCounter =0;
    char token[LINE_LENGTH]= ""; /*holds current word*/
    char cur_line[LINE_LENGTH]= "";  /*holds current line*/
    FILE* readFP; /*file pointer to read .am file */
    if((readFP = fopen(filename,"r"))==NULL)
        return false;   /* ^ file doesn't exist*/
    while(fgets(cur_line,LINE_LENGTH,readFP)){
        sscanf(cur_line,WORD_FORMAT,token);
        if(token[strlen(token)-1]==':'){   /*if a symbol was anounced*/
            line = newSymbol(token, lineCounter,0,0,atr);
            insertSymbol(line,chart);
        }
        memset(token, 0, sizeof(token));
    }
    fclose(readFP);
    printf("Finished pass1 function !\n");
    return true;
}

Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]){
    Line* line = (Line *)malloc(sizeof(Line));
    if (!line){
        printf("alocation error !\n");
        exit(0);
    }
    strcpy(line->symbol,symbol);
    line->value = value;
    line->offset = offset;
    line->baseAdrs = baseAdrs;
    copyArr(attribute,line->attributes);
    printf("\nNEW SYMBOL  \n\n");
    return line;
}

void copyArr(bool from[4], bool to[4]){
    int i;
    for(i=0;i<4;i++)
        to[i]=from[i];
}
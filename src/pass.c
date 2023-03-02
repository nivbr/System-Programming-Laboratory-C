#include "symbolChart.h"
#define WORD_FORMAT " %80s"
#define FIRST_MEM_CELL 100


Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]);  
bool pass1(char* filename ,symbolChart * chartptr);

bool pass(char** filesList[4],int listCounters[4]){
    symbolChart * chart;
    int i;
    printf(">Parsing process\n\n");
    for (i=0;i<listCounters[afterMacro];i++){
        printf(">Parsing [%s]\n",filesList[afterMacro][i]);
        chart = newSymbolChart();
        pass1(filesList[afterMacro][i],chart);
        printSymbolChart(chart);
        deleteSymbolChart(chart);
        printf(">End Parsing [%s]\n\n",filesList[afterMacro][i]);
    }
    printf("\n\n>End Parsing process\n\n");
    return true;
}

bool pass1(char* filename ,symbolChart * chart){
    Line * line = NULL;
    bool atr[4] = {false,false,false,false};
    int lineCounter =0, IC=0,DC=0;
    char token[LINE_LENGTH]= ""; /*holds current word*/
    char cur_line[LINE_LENGTH]= "";  /*holds current line*/
    FILE* readFP; /*file pointer to read .am file */
    printf("\n\t>Pass 1 [%s]\n",filename);
    if((readFP = fopen(filename,"r"))==NULL)
        return false;   /* ^ file doesn't exist*/
    while(fgets(cur_line,LINE_LENGTH,readFP)){
        lineCounter++;
        sscanf(cur_line,WORD_FORMAT,token);
        if(token[strlen(token)-1]==':'){   /*if a symbol was anounced*/
            line = newSymbol(token, lineCounter,0,0,atr);
            insertSymbol(line,chart);
        }
        memset(token, 0, sizeof(token));
    }
    fclose(readFP);
    printf("\t>End Pass 1 [%s]\n\n",filename);
    return true;
}

Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]){
    int i;
    Line* line = (Line *)malloc(sizeof(Line));
    if (!line){
        printf("alocation error !\n");
        exit(0);
    }
    strcpy(line->symbol,symbol);
    line->value = value;
    line->offset = offset;
    line->baseAdrs = baseAdrs;
    for(i=0;i<4;i++)
        line->attributes[i]=attribute[i];
    return line;
}
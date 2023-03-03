#include "symbolChart.h"
#include <ctype.h>

#define WORD_FORMAT " %80s"
#define FIRST_MEM_CELL 100

Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]);
bool pass1(char* filename ,symbolChart * chart, int dataMem[256][2], int codeMem[256][2]); /*return true if no errors !*/
void doLine(char* cur_line,int* IC, int* DC, symbolChart * chart, int dataMem[256][2], int codeMem[256][2], bool * errorFlag); /*return true if OK, false if error found*/
void clearString(char * s);

bool pass(char** filesList[4],int listCounters[4]){
    symbolChart * chart;
    int dataMem[256][2]={0};
    int codeMem[256][2]={0};
    int i;
    printf(">Parsing process\n\n");
    for (i=0;i<listCounters[afterMacro];i++){
        printf(">Parsing [%s]\n",filesList[afterMacro][i]);
        chart = newSymbolChart();
        if(pass1(filesList[afterMacro][i],chart,dataMem,codeMem)){  /*no error found*/
            printSymbolChart(chart);    /*debug print*/
            deleteSymbolChart(chart);
            /*pass2(...)*/
        }
        printf(">End Parsing [%s]\n\n",filesList[afterMacro][i]);
    }
    printf("\n\n>End Parsing process\n\n");
    return true;
}

bool pass1(char* filename ,symbolChart * chart, int dataMem[256][2], int codeMem[256][2]){
    char cur_line[LINE_LENGTH]= "";  /*holds current line*/
    int IC=0,DC=0;
    bool errorFlag= false;
    FILE* readFP; /*file pointer to read .am file */
    printf("\n\t>Pass 1 [%s]\n",filename);
    if((readFP = fopen(filename,"r"))==NULL)
        return false;   /* ^ file doesn't exist*/
    while(fgets(cur_line,LINE_LENGTH,readFP))  /*foreach line in the file*/
        doLine(cur_line,&IC,&DC,chart,dataMem,codeMem,&errorFlag);
    fclose(readFP);
    printf("\t>End Pass 1 [%s]\n\n",filename);
    if(errorFlag)
        return false;
    /*else: update all {type==data} in table by addinf IC to value*/
    return true;
}

void doLine(char* cur_line,int* IC, int* DC, symbolChart * chart, int dataMem[256][2], int codeMem[256][2], bool * errorFlag){
    Line * line = NULL;
    bool atr[4] = {false,false,false,false}, symbolFlag= false;   /*errorFlag is on if errors found- the pass2 won't happen*/
    bool flagData= false, flagString=false;
    int L=0;
    char token[LINE_LENGTH]= ""; /*holds current word*/
    char symbol[LINE_LENGTH]="";    /*holds the symbol if there is*/
    char tempLine[LINE_LENGTH]="";
    sscanf(cur_line,WORD_FORMAT,token); /*get first token*/
    strcpy(tempLine,cur_line);
    if(!strcmp(token,";")) /*ignore line that starts with ';' */
        return;
    /*if a symbol was anounced*/
    if(token[strlen(token)-1]==':' && !strcmp(token,":")){
        symbolFlag= true;
        strcpy(symbol,token);               /*put token into symbol*/
        strcpy(token,strtok(tempLine,token));   /*update token to be the rest of the line*/
        strtok(token," ");    /*put the seconde token as first*/

    }else{strcpy(symbol,"");}   /*reset symbol*/
    /*if there was a symbol definition: then symbol holds it and token holds the first op*/
    /*else:  symbol holds nothing and token holds the first op*/
    
    if(!strcmp(token,".data") || !strcmp(token,".string")){ /*data type*/
        flagData=false;
        flagString=false;
        if(!strcmp(token,".data"))  /*.data*/
            flagData=true;
        else                        /*.string*/
            flagString=true;
        
        if (symbolFlag){
            /**/;
        }
        /*define data*/
        /*write data to mem*/
        /*update DC (by last stages)*/
        

    }else if(!strcmp(token,".extern")){ /*external (OK to assume: no lables before)*/        
        /*getNextToken(cur_line,token);*/
        strcpy(token, strtok(tempLine, " "));
        strcpy(token , strtok(NULL, ","));
        clearString(token);
        /*insert to symbolChart */
        atr[external]=true; /*set the attributes to to external for it to be copied*/            
        line = newSymbol(token,0,0,0,atr);
        atr[external]=false;    /*was COPIED so can be reset*/
        insertSymbol(line,chart);   /*insert to chart*/               
    }else if(!strcmp(token,".entry")){  /*entry- will be done by pass2*/
        return;
    }else{  /*just code*/
        /*insert to table if needed
        lookup for op in chart (error or OK)
        -Decode op struct
        CALCULATE L !
        Build Bin Code for 1st word*/;
        *IC +=L;
    }
    return;
}

void clearString(char * s){
    int i;
    char* t=s;
    for(i=strlen(s)-1;i>=0;i--) /*clear ending*/
        if(!isalnum(s[i]))
            s[i]=0;
        else break;
    for(i=0;i<strlen(s);i++)
        if(!isalnum(s[i]))
            t++;
        else break;
    strcpy(s,t);
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

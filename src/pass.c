#include "symbolChart.h"
#include <ctype.h>

#define FIRST_MEM_CELL 100

Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]);
bool pass1(char* filename ,symbolChart * chart, int dataMem[256], int codeMem[MEMORY_SIZE]); /*return true if no errors !*/
void doLine(char* cur_line,int* IC, int* DC, symbolChart * chart, int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE], bool * errorFlag); /*return true if OK, false if error found*/
void clearString(char * s);
bool stringIsEmpty(char* s);
void printMemPic(int mem[MEMORY_SIZE], const char* headline);

bool pass(char** filesList[4],int listCounters[4]){
    symbolChart * chart;
    int dataMem[256]={0};
    int codeMem[256]={0};
    int i;
    printf(">Parsing process\n\n");
    for (i=0;i<listCounters[afterMacro];i++){
        printf(">Parsing [%s]\n",filesList[afterMacro][i]);
        chart = newSymbolChart();
        if(pass1(filesList[afterMacro][i],chart,dataMem,codeMem)){  /*no error found*/
            printSymbolChart(chart);    /*debug print*/
            deleteSymbolChart(chart);
            printMemPic(dataMem,"data");
            /*pass2(...)*/
        }
        printf(">End Parsing [%s]\n\n",filesList[afterMacro][i]);
    }
    printf("\n\n>End Parsing process\n\n");
    return true;
}

bool pass1(char* filename ,symbolChart * chart, int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE]){
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

void doLine(char* cur_line,int* IC, int* DC, symbolChart * chart, int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE], bool * errorFlag){
    Line * line = NULL;
    bool atr[4] = {false,false,false,false}, symbolFlag= false;   /*errorFlag is on if errors found- the pass2 won't happen*/
    int L=0;
    char* token = NULL;
    char symbol[LINE_LENGTH]="";    /*holds the symbol if there is*/
    token = strtok(cur_line," ");   /*get first token*/
    if(!strcmp(token,";")) /*ignore line that starts with ';' */
        return;
    /*if a symbol was anounced*/
    if(token[strlen(token)-1]==':' && strcmp(token,":")){
        printf("\n\tLABEL: [%s] FOUND\n\n",token);
        symbolFlag= true;
        strcpy(symbol,token);               /*put token into symbol*/
        /*strcpy(token,strtok(NULL," "));*/   /*update token to be the rest of the line*/
        token = strtok(NULL," ");

    }else{strcpy(symbol,"");}   /*reset symbol*/
    /*if there was a symbol definition: then symbol holds it and token holds the first op*/
    /*else:  symbol holds nothing and token holds the first op*/    
    clearString(token);
    if(!strcmp(token,".data") || !strcmp(token,".string")){ /*data type*/
        L=0;
        if(!strcmp(token,".data")){  /*.data*/
            if(symbolFlag){
                symbolFlag=false;
                if(searchSymbol(chart,symbol))  /*if symbol already in chart*/
                    *errorFlag=true;
                else{
                    atr[data]=true; /*set the attributes to to external for it to be copied*/            
                    line = newSymbol(symbol,*DC,0,0,atr);
                    atr[data]=false;    /*was COPIED so can be reset*/
                    insertSymbol(line,chart);   /*insert to chart*/
                }
            }
            printf("\n\tBEFORE: IN DATA-> token: [%s] ,line:[%s] \n\n",token,cur_line);        
            while(!stringIsEmpty(token)){
                printf("\n\tAdd: [%d][%d]\n",(*DC),atoi(token));
                dataMem[*DC]= atoi(token);
                *DC += 1;
                L++;
                token = strtok(NULL,",");
            }
        }
        else{    /*.string*/
            ;
        }
        if (symbolFlag && false){    /*sure 'code' attribute is OK?*/
            symbolFlag=false;
            if(searchSymbol(chart,symbol))  /*if symbol already in chart*/
                *errorFlag=true;
            else{
                atr[code]=true; /*set the attributes to to external for it to be copied*/            
                line = newSymbol(symbol,*IC,0,0,atr);
                atr[data]=false;    /*was COPIED so can be reset*/
                insertSymbol(line,chart);   /*insert to chart*/
                /*calculate L*/
                *IC+=L;
            }
        }
        /*define data*/
        /*write data to mem*/
        /*update DC (by last stages)*/

    }else if(!strcmp(token,".extern")){ /*external (OK to assume: no lables before)*/        
        token = strtok(NULL," ");
        clearString(token);
        /*insert to symbolChart */
        atr[external]=true; /*set the attributes to to external for it to be copied*/            
        line = newSymbol(token,0,0,0,atr);
        atr[external]=false;    /*was COPIED so can be reset*/
        insertSymbol(line,chart);   /*insert to chart*/
        memset(token,0,strlen(token));
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
        if(isspace(s[i]))
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

bool stringIsEmpty(char* s){
    int i;
    if(!s)
        return true;
    for(i=0;i<strlen(s);i++)
        if(!isspace(s[i]))
            return false;
    return true;
}

void printMemPic(int mem[MEMORY_SIZE], const char* headline){
    int i;
    printf("\t--%s memory map--\n",headline);
    for(i=0;i<MEMORY_SIZE;i++)
        if(mem[i]!=0)
            printf("\t\t%d | %d\n",i,mem[i]);
    printf("\t----------------------\n\n");
}
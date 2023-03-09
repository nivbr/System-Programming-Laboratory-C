#include "pass.h"

char ops[OP_COUNT][OP_MAX_LENGTH]={{"mov"},{"cmp"},{"add"},{"sub"},{"not"},{"clr"},{"lea"},{"inc"},{"dec"},{"jmp"},{"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"}};

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
            /*printMemPic(codeMem,"code");*/
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
    int L=0,i;
    char* token = NULL;
    char symbol[LINE_LENGTH]="";    /*holds the symbol if there is*/
    token = strtok(cur_line," ");   /*get first token*/
    if(!strcmp(token,";")) /*ignore line that starts with ';'- maybe change with token[0] because can be ';blalba' and then strcmp wont return 0 */
        return;
    /*if a symbol was anounced*/
    if(token[strlen(token)-1]==':' && strcmp(token,":")){
        symbolFlag= true;
        strcpy(symbol,token);   /*put token into symbol*/
        clearString(symbol);
        token = strtok(NULL," ");
    }else{strcpy(symbol,"");}   /*reset symbol*/
    /*if there was a symbol definition: then symbol holds it and token holds the first op*/
    /*else:  symbol holds nothing and token holds the first op*/    
    clearString(token);
    /*data OR string type*/
    if(!strcmp(token,".data") || !strcmp(token,".string")){         
        /*WITH symbol lable */        
        if (symbolFlag){    /*sure 'code' attribute is OK?*/
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
        /*.data*/
        if(!strcmp(token,".data")){
            token = strtok(NULL,",");   /*???????*/
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
            clearString(token);   
            printf("\n\ttoken to memory: %s\n\n",token);
            while(!stringIsEmpty(token)){                
                dataMem[*DC]= atoi(token);
                (*DC)++;                
                token = strtok(NULL,",");
            }
        }
        else{ /*.string*/
            token = strtok(NULL,", \"");   
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
            for(i=0;i<strlen(token);i++)
                dataMem[(*DC)++]=token[i];
        }
    /*external (OK to assume: no lables before)*/        
    }else if(!strcmp(token,".extern")){ 
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
    /*just code*/
    }else{  
         if(symbolFlag){
            symbolFlag=false;
            if(searchSymbol(chart,symbol))  /*if symbol already in chart*/
                *errorFlag=true;
            else{
                atr[code]=true; /*set the attributes to to external for it to be copied*/            
                line = newSymbol(symbol,*IC,0,0,atr);
                atr[code]=false;    /*was COPIED so can be reset*/
                insertSymbol(line,chart);   /*insert to chart*/
            }
        }
        for(i=0;i<OP_COUNT;i++)     /*check if op is legal*/
            if(!strcmp(ops[i],token))
                break;
        if(i>OP_COUNT)
            *errorFlag=true;    /*add error treatment*/
        /*
        -Decode op struct
        -CALCULATE L !
        -Build Bin Code for 1st word
        */
        (*IC) +=L;
    }
    return;
}

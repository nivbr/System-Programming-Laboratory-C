#include "pass.h"

char opsA[OP_COUNT][OP_MAX_LENGTH]={{"mov"},{"cmp"},{"add"},{"sub"},{"not"},{"clr"},{"lea"},{"inc"},{"dec"},{"jmp"},{"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"}};

void doLine1(char* cur_line,int* IC, int* DC, symbolChart * chart, int dataMem[MEMORY_SIZE], bool * errorFlag, int lineCounter);
void doLine2(char* cur_line,int* IC, int* DC, symbolChart * chart,LinkedList* extApperance, int codeMem[MEMORY_SIZE], bool * errorFlag,int lineCounter);
bool pass1(char* filename ,symbolChart * chart,int *codeMemSize ,int *dataMemSize,  int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE]);
bool pass2(char* filename ,symbolChart * chart,LinkedList* extApperance,int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE]);

bool pass(char** filesList[4],int listCounters[4]){
    symbolChart * chart;
    LinkedList * extApperance;
    int dataMem[MEMORY_SIZE]={0};
    int codeMem[MEMORY_SIZE]={0};
    int i, codeMemSize=0,dataMemSize=0;
    printf(">Parsing process\n\n");
    for (i=0;i<listCounters[afterMacro];i++){
        printf(">Parsing [%s]\n",filesList[afterMacro][i]);
        chart = newSymbolChart();
        extApperance = newList();
        if(pass1(filesList[afterMacro][i],chart,&codeMemSize,&dataMemSize,dataMem,codeMem)){  /*no error found*/
            printSymbolChart(chart);    /*debug print*/
            /*printMemPic(dataMem,"data");*/
            if(pass2(filesList[afterMacro][i],chart,extApperance,dataMem,codeMem)){  /*no error found*/
                printSymbolChart(chart);    /*debug print*/
                printMemPic(codeMem,codeMemSize,"code");
                printMemPic(dataMem,dataMemSize,"data");
                createObFile(filesList[afterMacro][i],codeMemSize,dataMemSize,codeMem,dataMem);
                createEntFile(filesList[afterMacro][i],chart);
                createExtFile(filesList[afterMacro][i],extApperance);
            }
        }
        deleteSymbolChart(chart);
        deleteList(extApperance);
        printf(">End Parsing [%s]\n\n",filesList[afterMacro][i]);
    }
    printf("\n\n>End Parsing process\n\n");
    return true;
}

bool pass1(char* filename ,symbolChart * chart,int *codeMemSize ,int *dataMemSize,  int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE]){
    char cur_line[LINE_LENGTH]= "";  /*holds current line*/
    int IC=0,DC=0, lineCounter=0;
    bool errorFlag= false;
    FILE* readFP; /*file pointer to read .am file */
    printf("\n\t>Pass 1 [%s]\n",filename);
    if((readFP = fopen(filename,"r"))==NULL)
        return false;   /* ^ file doesn't exist*/
    while(fgets(cur_line,LINE_LENGTH,readFP)){  /*foreach line in the file*/
        strip_extra_spaces(cur_line);        
        doLine1(cur_line,&IC,&DC,chart,dataMem,&errorFlag,++lineCounter);
    }
    fclose(readFP);
    *codeMemSize=IC;
    *dataMemSize=DC;
    printf("\t>End Pass 1 [%s]\n\n",filename);
    if(errorFlag){
        printf("\tERRORS ACUURED!\n");
        return false;
    }
    printf("\tNO ERRORS!\n");
    updateDataByIC(chart,IC);
    return true;
}

void doLine1(char* cur_line,int* IC, int* DC, symbolChart * chart, int dataMem[MEMORY_SIZE], bool * errorFlag, int lineCounter){
    Line * line = NULL;
    bool atr[4] = {false,false,false,false}, symbolFlag= false;   /*errorFlag is on if errors found- the pass2 won't happen*/
    int i,L=0;
    char* token = NULL;
    char symbol[LINE_LENGTH]="";    /*holds the symbol if there is*/
    char copy_line[LINE_LENGTH]="";
    strcpy(copy_line,cur_line);
    token = strtok(cur_line," \t");   /*get first token*/
    if(!strcmp(token,";") || stringIsEmpty(copy_line) || token[0]==';') /*ignore line that starts with ';'- maybe change with token[0] because can be ';blalba' and then strcmp wont return 0 */
        return;
    clearString(token);
    /*if a symbol was anounced*/
    if(token[strlen(token)-1]==':' && strcmp(token,":")){
        token[strlen(token)-1]='\0';   /*enter symbol without ':' sign*/
        clearString(token);
        symbolFlag= true;
        strcpy(symbol,token);   /*put token into symbol*/        
        if(isSymbolError(symbol)){
            *errorFlag = true;
            printf("ERROR: (Line #%d) symbol:[%s] CAN'T BE OP/REG/NON-ALFA-NUMERIC!\n",lineCounter,symbol);
        }
        token = strtok(NULL," ");
        if(!strcmp(token,":"))  /*if lable and ':' had space between them*/
            *errorFlag = true;
    }else{strcpy(symbol,"");}   /*reset symbol*/
    /*if there was a symbol definition: then symbol holds it and token holds the first op*/
    /*else:  symbol holds nothing and token holds the first op*/    
    clearString(token);
    /*data OR string type*/
    if(!strcmp(token,".data") || !strcmp(token,".string")){         
        /*WITH symbol lable */        
        if (symbolFlag){    /*sure 'code' attribute is OK?*/
            symbolFlag=false;
            if(searchSymbol(chart,symbol)){  /*if symbol already in chart*/
                *errorFlag=true;
                printf("ERROR: (Line #%d) symbol:[%s] ALLREADY ANNOUNCED!\n",lineCounter,symbol);
            }
            else{
                atr[data]=true; /*set the attributes to to external for it to be copied*/            
                line = newSymbol(symbol,*DC,0,0,atr);
                atr[data]=false;    /*was COPIED so can be reset*/        
                insertSymbol(line,chart);   /*insert to chart*/
            }
        }
        /*.data*/
        if(!strcmp(token,".data")){
            token = strtok(NULL,",");
            if(symbolFlag){
                symbolFlag=false;
                if(searchSymbol(chart,symbol)){  /*if symbol already in chart*/
                    *errorFlag=true;
                    printf("ERROR: (Line #%d) symbol:[%s] ALLREADY ANNOUNCED!\n",lineCounter,symbol);
                }
                else{
                    atr[data]=true; /*set the attributes to to external for it to be copied*/            
                    line = newSymbol(symbol,*DC,0,0,atr);
                    atr[data]=false;    /*was COPIED so can be reset*/        
                    insertSymbol(line,chart);   /*insert to chart*/
                }
            }
            clearString(token);   
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
                if(searchSymbol(chart,symbol)){  /*if symbol already in chart*/
                    *errorFlag=true;
                    printf("ERROR: (Line #%d) symbol:[%s] ALLREADY ANNOUNCED!\n",lineCounter,symbol);
                }
                else{                    
                    atr[data]=true; /*set the attributes to to external for it to be copied*/            
                    line = newSymbol(symbol,*DC,0,0,atr);
                    atr[data]=false;    /*was COPIED so can be reset*/
                    insertSymbol(line,chart);   /*insert to chart*/
                }
            }
            for(i=0;i<strlen(token);i++)
                dataMem[(*DC)++]=token[i];
            dataMem[(*DC)++]='\0';
        }
    /*external (OK to assume: no lables before)*/        
    }else if(!strcmp(token,".extern")){ 
        token = strtok(NULL," ");
        clearString(token);
        /*insert to symbolChart */
        atr[external]=true; /*set the attributes to to external for it to be copied*/            
        line = newSymbol(token,0,0,0,atr);
        atr[external]=false;    /*was COPIED so can be reset*/
        if(searchSymbol(chart,token)){    /*if symbol already anounced*/
            *errorFlag=true;
            printf("ERROR: (Line #%d) symbol:[%s] ALLREADY ANNOUNCED!\n",lineCounter,symbol);
        }
        else
            insertSymbol(line,chart);   /*insert to chart*/
        memset(token,0,strlen(token));
    }else if(!strcmp(token,".entry")){  /*entry- will be done by pass2*/
        return;
    }else{  /*just code*/
        if(symbolFlag){
            if(searchSymbol(chart,symbol)){  /*if symbol already in chart*/
                *errorFlag=true;
                printf("ERROR: (Line #%d) symbol:[%s] ALLREADY ANNOUNCED!\n",lineCounter,symbol);
            }
            else{
                atr[code]=true; /*set the attributes to to external for it to be copied*/            
                line = newSymbol(symbol,*IC,0,0,atr);
                atr[code]=false;    /*was COPIED so can be reset*/
                insertSymbol(line,chart);   /*insert to chart*/
            }
        }
        clearString(token);
        for(i=0;i<OP_COUNT;i++)     /*check if op is legal*/
            if(!strcmp(opsA[i],token))
                break;
        if(i<OP_COUNT)
            L = calcL(copy_line,symbolFlag,errorFlag,lineCounter);        
        else{
            *errorFlag=true;    /*add error treatment*/
            printf("ERROR: (Line #%d) op:[%s] NOT FOUND!\n",lineCounter,token);
        }
        (*IC) += L;
        symbolFlag = false;
    }
    return;
}

bool pass2(char* filename ,symbolChart * chart,LinkedList* extApperance,int dataMem[MEMORY_SIZE], int codeMem[MEMORY_SIZE]){
    char cur_line[LINE_LENGTH]= "";  /*holds current line*/
    int IC=0,DC=0,lineCounter=0;
    bool errorFlag= false;
    FILE* readFP; /*file pointer to read .am file */
    printf("\n\t>Pass 2 [%s]\n",filename);
    if((readFP = fopen(filename,"r"))==NULL)
        return false;   /* ^ file doesn't exist*/
    while(fgets(cur_line,LINE_LENGTH,readFP)){  /*foreach line in the file*/
        strip_extra_spaces(cur_line);
        doLine2(cur_line,&IC,&DC,chart,extApperance,codeMem,&errorFlag,++lineCounter);
    }
    fclose(readFP);
    printf("\t>End Pass 2 [%s]\n\n",filename);
    if(errorFlag)
        return false;    
    return true;
}

void doLine2(char* cur_line,int* IC, int* DC, symbolChart * chart,LinkedList* extApperance, int codeMem[MEMORY_SIZE], bool * errorFlag,int lineCounter){
    Line * line = NULL;
    char* token = NULL;
    char symbol[LINE_LENGTH]="";    /*holds the symbol if there is*/
    char copy_line[LINE_LENGTH]="";
    bool symbolFlag = false;
    strcpy(copy_line,cur_line);
    token = strtok(cur_line," \t");   /*get first token*/
    if(!strcmp(token,";") || stringIsEmpty(copy_line) || token[0]==';') /*ignore line that starts with ';'- maybe change with token[0] because can be ';blalba' and then strcmp wont return 0 */
        return;
    clearString(token);
    /*if a symbol was anounced*/
    if(token[strlen(token)-1]==':' && strcmp(token,":")){
        token[strlen(token)-1]='\0';   /*enter symbol without ':' sign*/
        clearString(token);
        symbolFlag= true;
        strcpy(symbol,token);   /*put token into symbol*/        
        token = strtok(NULL," ");
    }else{strcpy(symbol,"");}   /*reset symbol*/
    /*if there was a symbol definition: then symbol holds it and token holds the first op*/
    /*else:  symbol holds nothing and token holds the first op*/    
    /*clearString(token);*/
    if(!strcmp(".entry",token)){    /*if an entry*/
        token = strtok(NULL," ");
        clearString(token);
        line = searchSymbol(chart, token);
        if(!line || line->attributes[external]){
            printf("ERROR: (line:%d) Cannot define lable as both .entery and .exter!\n",lineCounter);
            *errorFlag=true;
        }
        else
            line->attributes[entry]=true;
    }else  /*complete the coding and procseed L*/
        decode(copy_line,symbolFlag,chart,extApperance,IC,codeMem,lineCounter,errorFlag);
}
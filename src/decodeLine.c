#include "decodeLine.h"

char opsB[OP_COUNT][OP_MAX_LENGTH]={{"mov"},{"cmp"},{"add"},{"sub"},{"not"},
                                    {"clr"},{"lea"},{"inc"},{"dec"},{"jmp"},
                                    {"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"}};
void decodeObv(OpWord *opword, int opNum);
void decodeShita0(char* word,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart);
void decodeShita1(char* word,int *L ,int codeMem[MEMORY_SIZE], symbolChart *chart,LinkedList* extApperance, int lineCounter, bool * errorFlag);
void decodeShita2(char* word1, char* word2,int *L ,int codeMem[MEMORY_SIZE], symbolChart *chart,LinkedList* extApperance,int lineCounter, bool* errorFlag);
void decodeShita3(char* word, bool dst,int* L, int codeMem[MEMORY_SIZE], symbolChart *chart);   /*dst==true means op=dst, else op=src */
int shitaNum2parNum(int shita);
void decodeTwoRegs(char* reg1, char* reg2,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart);

/*decode the line and write it to mem*/
void decode(char line[LINE_LENGTH],bool startWLable, symbolChart * chart,LinkedList* extApperance, int *L, int codeMem[MEMORY_SIZE],int lineCounter, bool * errorFlag){
    int i,opNum=-1,shita=-1;
    char* token = NULL, *lbl = NULL, *p1=NULL, *p2=NULL;
    char copyLine[LINE_LENGTH]="";
    char *temp = "";
    OpWord* opword = (OpWord*)malloc(sizeof(OpWord));
    if (!opword){
        fprintf(stdout,"Allocation Error!\n");
        exit(1);
    }
    strcpy(copyLine,line);
    token = strtok(line, " \t");   /*initialize line to be tokened*/
    clearString(token);
    if(startWLable)
        token = strtok(NULL," \t");   /*skip the lable to start with op*/        
    clearString(token);

    for(i=0;i<OP_COUNT;i++)    /*find the opcode field*/
        if(!strcmp(token,opsB[i]))
            opNum=i; 
    decodeObv(opword,opNum);   
    /*0 operands*/
    if(opNum>=14){ /*rts,stop known because no parameters*/
        temp = strtok(NULL," ,\t");
        if(temp){  /*if ther's another token after the last one than it's an error*/
            *errorFlag=true;
            fprintf(stdout,"ERROR: (line:%d) too many operands !\n",lineCounter);            
        }else
            codeMem[(*L)++] = opWord2int(opword);    
    }
    else if(opNum==4 || opNum==5 || opNum==7 || opNum==8 || opNum==11 || opNum==12){ /*1 operand group*/
        token = strtok(NULL, " \t");  /*\t*/
        temp = strtok(NULL," ,\t");
        if(temp){  /*if ther's another token after the last one than it's an error*/
            *errorFlag=true;
            fprintf(stdout,"ERROR: (line:%d) too many operands !\n",lineCounter);
        }else{
            clearString(token);
            shita = token2op(token);
            opword->dst= shita;        
            codeMem[(*L)++] = opWord2int(opword);
            if(shita==IMMIDIATE /*0*/){
                if(opNum==12)
                    decodeShita0(token,L,codeMem,chart);
                else{
                    *errorFlag=true;                    
                    fprintf(stdout,"ERROR: (line:%d) operand illegal to this function !\n",lineCounter);                    
                }
            }
            else if(shita==LABLE   /*1*/)
                decodeShita1(token,L,codeMem,chart, extApperance,lineCounter,errorFlag);
            else if (shita==REGISTER   /*3*/)
                decodeShita3(token,true,L,codeMem,chart);
            else{
                *errorFlag=true;                
                fprintf(stdout,"ERROR: (line:%d) too many operands !\n",lineCounter);                
            }
        }
    }else if((opNum>=0 && opNum<=3) || opNum==6){     /*2 operands group*/
        p1 = strtok(NULL, " ,\t");  /*\t*/
        p2 = strtok(NULL," ,\t");   /*\t*/
        temp = strtok(NULL," ,\t");
        if(temp){  /*if ther's another token after the last one than it's an error*/
            *errorFlag=true;            
            fprintf(stdout,"ERROR: (line:%d) too many operands !\n",lineCounter);            
        }else{
            printf("\t2op group: p1:%s ,p2:%s\n",p1,p2);
            shita = token2op(p1);
            opword->src = token2op(p1);
            opword->dst = token2op(p2);        
            codeMem[(*L)++] = opWord2int(opword);
            if (shita == REGISTER && token2op(p2)==REGISTER)
                decodeTwoRegs(p1,p2,L,codeMem,chart);
            else{
                if(shita==IMMIDIATE /*0*/)
                    decodeShita0(p1,L,codeMem,chart);
                else if(shita==LABLE   /*1*/)
                    decodeShita1(p1,L,codeMem,chart, extApperance,lineCounter,errorFlag);
                else if (shita==REGISTER   /*3*/)
                    decodeShita3(p1,false,L,codeMem,chart);
                shita = token2op(p2);
                if(shita==IMMIDIATE /*0*/){
                    if(opNum==1)
                        decodeShita0(p2,L,codeMem,chart);
                    else{
                        *errorFlag=true;                        
                        fprintf(stdout,"ERROR: (line:%d) operand illegal to this function !\n",lineCounter);                        
                    }
                }
                else if(shita==LABLE   /*1*/)
                    decodeShita1(p2,L,codeMem,chart,extApperance,lineCounter,errorFlag);
                else if (shita==REGISTER   /*3*/)
                    decodeShita3(p2,true,L,codeMem,chart);
                else{
                    *errorFlag=true;                    
                    fprintf(stdout,"ERROR: (line:%d) operands do NOT match function !\n",lineCounter);                    
                }
            }
        }
    }else if(opNum==8 || opNum==9 || opNum==10 || opNum==13){  /*jmp group: only lable or lable w\ 2 parameters*/
        lbl = strtok(NULL, " (");
        /*if line continues after the lable then expect (par1,par2)*/
        if(isStringCont(strstr(copyLine,lbl), strlen(lbl) )){ 
            p1= strtok(NULL,",");
            p2 = strtok(NULL,",)"); 
            temp = strtok(NULL," ,\t");                       
            if(!p2){  /*if the second op is empty than lable + one op is illeagle*/
                *errorFlag=true;                
                fprintf(stdout,"ERROR: (line:%d)  too many operands!\n",lineCounter);                
            }else{                
                opword->par1=shitaNum2parNum(token2op(p1));
                opword->par2=shitaNum2parNum(token2op(p2));
                opword->dst=2;            
                codeMem[(*L)++]=opWord2int(opword);
                decodeShita1(lbl,L,codeMem,chart,extApperance,lineCounter,errorFlag);
                decodeShita2(p1,p2,L,codeMem,chart,extApperance,lineCounter,errorFlag);
            }
        }else{  /*don't expect more parameters after lable*/
            if(!temp){  /*if ther's another token after the last one than it's an error*/
                *errorFlag=true;                
                fprintf(stdout,"ERROR: (line:%d) )too many operands !\n",lineCounter);                
            }else{
                opword->dst=1;
                opword->par1=0;
                opword->par2=0;                            
                codeMem[(*L)++]=opWord2int(opword);
                decodeShita1(lbl,L,codeMem,chart,extApperance,lineCounter,errorFlag);
            }
        }
    }
    free(opword);
}
/*decode the obvious part of the word*/
void decodeObv(OpWord *opword, int opNum){
    opword->opcode=opNum;
    opword->ERA=0;
    if((opNum>=0 && opNum<=8)||opNum==15||opNum==14||opNum==12||opNum==11){
        opword->par1=0;
        opword->par2=0;
    }    
    if(opNum>=4 && opNum!=6)  
        opword->src=0;            
    if(opNum>=14)   /*group 3: 0 ops, regardless shitat miun*/
        opword->dst=0;
}
/*immidiate (#num)*/
void decodeShita0(char* word,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart){
    word++;
    print_binary(4 * atoi(word));
    codeMem[(*L)++]= 4 * atoi(word);  /*++word*/
}
/*lable*/
void decodeShita1(char* word,int *L ,int codeMem[MEMORY_SIZE], symbolChart *chart,LinkedList* extApperance, int lineCounter, bool * errorFlag){
    Line* sym = NULL;
    WordShita1* mila = (WordShita1*)malloc(sizeof(WordShita1));
    clearString(word);
    if(!mila){        
        fprintf(stdout,"Allocation Error !\n");        
        exit(1);
    }
    if((sym = searchSymbol(chart,word))){   /*if symbol on chart*/
        if(sym->attributes[external]){
            mila->ERA=1;
            addToList(sym->symbol,(*L)+FIRST_MEM_CELL,extApperance); /*if decoding an entry then add it to entry apperance map*/
        }            
        else if(sym->attributes[code] || sym->attributes[data] ){
            mila->ERA=2;
            mila->rest= sym->value + FIRST_MEM_CELL;    /*100 offset for starting memory count ??should i add by L ????????????*/            
        }
        else            
            fprintf(stdout,"Eror understading the decoding of shita 1 [word=%s]",word);            
    }else{                
        fprintf(stdout,"ERROR: (line:%d) can not refer to symbol that isnt appear in this file!\n",lineCounter);        
        *errorFlag=true;
        free(mila);
        return;
    }
    print_binary(mila->ERA + 4*mila->rest);
    codeMem[(*L)++] = mila->ERA + 4*mila->rest;
    free(mila);
}
/*the lable already decode seperatly before*/
void decodeShita2(char* word1, char* word2,int *L ,int codeMem[MEMORY_SIZE], symbolChart *chart,LinkedList* extApperance,int lineCounter, bool* errorFlag){
    int shita=-1;
    if(token2op(word1)==REGISTER && token2op(word2)==REGISTER)
        decodeTwoRegs(word1,word2,L,codeMem,chart);
    else{
        shita = token2op(word1);
        if(shita==IMMIDIATE /*0*/)
            decodeShita0(word1,L,codeMem,chart);
        else if(shita==LABLE   /*1*/)
            decodeShita1(word1,L,codeMem,chart,extApperance,lineCounter,errorFlag);
        else if (shita==REGISTER   /*3*/)
            decodeShita3(word1,false,L,codeMem,chart);
        
        shita = token2op(word2);
        if(shita==IMMIDIATE /*0*/)
            decodeShita0(word2,L,codeMem,chart);
        else if(shita==LABLE   /*1*/)
            decodeShita1(word2,L,codeMem,chart,extApperance,lineCounter,errorFlag);
        else if (shita==REGISTER   /*3*/)
            decodeShita3(word2,true,L,codeMem,chart);
    }
}
void decodeShita3(char* word,bool dst,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart){
    if(dst){ /*op is dst*/
        print_binary(4 * atoi(strstr(word,"r")+1));
        codeMem[(*L)++]= 4 * atoi(strstr(word,"r")+1);
    }
    else{    /*op is src*/
        print_binary(256 * atoi(strstr(word,"r")+1));
        codeMem[(*L)++]= 256 * atoi(strstr(word,"r")+1);
    }
}
void decodeTwoRegs(char* reg1, char* reg2,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart){
    clearString(reg1);clearString(reg2);
    print_binary(4*atoi(strstr(reg2,"r")+1) + 256*atoi(strstr(reg1,"r")+1));
    codeMem[(*L)++] = 4*atoi(strstr(reg2,"r")+1) + 256*atoi(strstr(reg1,"r")+1);
}
/*converte */
int shitaNum2parNum(int shita){
    if(shita==REGISTER /*3*/)
        return 3;
    return shita;
}

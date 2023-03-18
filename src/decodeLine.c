#include "decodeLine.h"

char opsB[OP_COUNT][OP_MAX_LENGTH]={{"mov"},{"cmp"},{"add"},{"sub"},{"not"},
                                    {"clr"},{"lea"},{"inc"},{"dec"},{"jmp"},
                                    {"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"}};
/*decodeShitaX() will increase L by its own*/
void decodeObv(OpWord *opword, int opNum);
void decodeShita0(char* word,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart);
void decodeShita1(char* word,int*L, int codeMem[MEMORY_SIZE], symbolChart *chart);
void decodeShita2(char* word1, char* word2,int *L ,int codeMem[MEMORY_SIZE], symbolChart *chart);
void decodeShita3(char* word, bool dst,int* L, int codeMem[MEMORY_SIZE], symbolChart *chart);   /*dst==true means op=dst, else op=src */
int shitaNum2parNum(int shita);
void decodeTwoRegs(char* reg1, char* reg2,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart);

void decode(char line[LINE_LENGTH],bool startWLable, symbolChart * chart, int *L, int codeMem[MEMORY_SIZE]){
    int i,opNum=-1,shita=-1;
    char* token = NULL, *lbl = NULL, *p1=NULL, *p2=NULL;
    OpWord* opword = (OpWord*)malloc(sizeof(OpWord));
    if (!opword){
        printf("Allocation Error!\n");
        exit(0);
    }
    /*printf("\n\tDecode: %s \n\n",line);*/

    token = strtok(line, " \t");   /*initialize line to be tokened*/
    clearString(token);
    if(startWLable)
        token = strtok(NULL," \t");   /*skip the lable to start with op*/        
    clearString(token);

    /*printf("\nDecode op: %s \n\n",token);*/

    for(i=0;i<OP_COUNT;i++)    /*find the opcode field*/
        if(!strcmp(token,opsB[i]))
            opNum=i; 
    decodeObv(opword,opNum);   /*0 operands*/
    if(opNum>=14) /*rts,stop known because no parameters*/
        codeMem[(*L)++] = opWord2int(opword);
    else if(opNum==4 || opNum==5 || opNum==7 || opNum==8 || opNum==11 || opNum==12){ /*1 operand group*/
        token = strtok(NULL, " ");
        clearString(token);
        shita = token2op(token);
        opword->dst= shita;
        /*opword->dst= token2op(strtok(NULL, " "));*/
        codeMem[(*L)++] = opWord2int(opword);
        if(shita==IMMIDIATE /*0*/)
            decodeShita0(token,L,codeMem,chart);
        else if(shita==LABLE   /*1*/)
            decodeShita1(token,L,codeMem,chart);
        else if (shita==REGISTER   /*3*/)
            decodeShita3(token,true,L,codeMem,chart);
    }else if((opNum>=0 && opNum<=3) || opNum==6){     /*2 operands group*/
        p1 = strtok(NULL, " ,");
        p2 = strtok(NULL," ,");
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
                decodeShita1(p1,L,codeMem,chart);
            else if (shita==REGISTER   /*3*/)
                decodeShita3(p1,false,L,codeMem,chart);
            shita = token2op(p2);
            if(shita==IMMIDIATE /*0*/)
            decodeShita0(p2,L,codeMem,chart);
            else if(shita==LABLE   /*1*/)
                decodeShita1(p2,L,codeMem,chart);
            else if (shita==REGISTER   /*3*/)
                decodeShita3(p2,true,L,codeMem,chart);
        }
    }else if(opNum==8 || opNum==9 || opNum==10 || opNum==13){  /*jmp group: only lable or lable w\ 2 parameters*/
        lbl = strtok(NULL, " (");
        if(isStringCont(line, lbl-line+1)){ /*if line continues after the lable then expect (par1,par2)*/
            p1= strtok(NULL,"(");
            p2 = strtok(NULL,",");
            opword->par1=shitaNum2parNum(token2op(p1));
            opword->par2=shitaNum2parNum(token2op(p2));
            opword->dst=token2op(lbl);
            codeMem[(*L)++]=opWord2int(opword);
            decodeShita1(lbl,L,codeMem,chart);
            decodeShita2(p1,p2,L,codeMem,chart);

        }else{  /*don't expect more parameters after lable*/
            opword->dst=1;
            opword->par1=0;
            opword->par2=0;
            codeMem[(*L)++]=opWord2int(opword);
            decodeShita1(lbl,L,codeMem,chart);
        }
    }
    free(opword);
}

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
    codeMem[(*L)++]= 4 * atoi(word);
}
/*lable*/
void decodeShita1(char* word,int *L ,int codeMem[MEMORY_SIZE], symbolChart *chart){
    Line* symbol = NULL;
    WordShita1* mila = (WordShita1*)malloc(sizeof(WordShita1));
    clearString(word);
    if(!mila){
        printf("Allocation Error !\n");
        exit(1);
    }
    if((symbol = searchSymbol(chart,word))){   /*if symbol on chart*/
        mila->rest= symbol->value;
        if(symbol->attributes[external])
            mila->ERA=1;
        else if(symbol->attributes[code] || symbol->attributes[data] )
            mila->ERA=2;
        else
            printf("Eror understading the decoding of shita 1 [word=%s]",word);
    }else{
        printf("Error symbol not found! [%s]\n",word);
        free(mila);
        return;
    }
    codeMem[(*L)++] = mila->ERA + 4*mila->rest;
    free(mila);
}
/*the lable already decode seperatly before*/
void decodeShita2(char* word1, char* word2,int *L ,int codeMem[MEMORY_SIZE], symbolChart *chart){
    int shita=-1;
    if(token2op(word1)==REGISTER && token2op(word2)==REGISTER)
        decodeTwoRegs(word1,word2,L,codeMem,chart);
    else{
        shita = token2op(word1);
        if(shita==IMMIDIATE /*0*/)
            decodeShita0(word1,L,codeMem,chart);
            else if(shita==LABLE   /*1*/)
                decodeShita1(word1,L,codeMem,chart);
            else if (shita==REGISTER   /*3*/)
                decodeShita3(word1,false,L,codeMem,chart);
            shita = token2op(word2);
            if(shita==IMMIDIATE /*0*/)
            decodeShita0(word2,L,codeMem,chart);
            else if(shita==LABLE   /*1*/)
                decodeShita1(word2,L,codeMem,chart);
            else if (shita==REGISTER   /*3*/)
                decodeShita3(word2,true,L,codeMem,chart);
    }
}
void decodeShita3(char* word,bool dst,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart){
    if(dst) /*op is dst*/
        codeMem[(*L)++]= 4 * atoi(word);
    else    /*op is src*/
        codeMem[(*L)++]= 256 * atoi(word);
}
void decodeTwoRegs(char* reg1, char* reg2,int *L, int codeMem[MEMORY_SIZE], symbolChart *chart){
    codeMem[(*L)++] = 4*atoi(reg1) + 256*atoi(reg2);
}

int shitaNum2parNum(int shita){
    if(shita==REGISTER /*3*/)
        return 2;
    return shita;
}

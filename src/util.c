#include "util.h"

char ops[OP_COUNT][OP_MAX_LENGTH]={{"mov"},{"cmp"},{"add"},{"sub"},{"not"},{"clr"},{"lea"},{"inc"},{"dec"},{"jmp"},{"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"}};
/*returns true if number is written legally (rounded)*/
bool checkNum(double num);
/*returns true if token stands for legall register number*/
bool regIsCorrect(char token[LINE_LENGTH]);

/*clear string from leading/+finishing whitespaces*/
void clearString(char * s){
    int i,start=0,finish=strlen(s);
    char temp[81]="";
    
    for(i=0;i<strlen(s);i++)
        if(!isspace(s[i])){
            start = i;
            break;
        }
    for(i=strlen(s)-1;i>=0;i--)
        if(!isspace(s[i])){
            finish = i;
            break;
        }
    for(i=0;i<finish-start+1;i++)
        temp[i]=s[start+i];
    strcpy(s,temp);
    s[i]='\0';  /*?????*/
}

/*create new symbol(=line) in symbol chart*/
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

/*returns true if string is empty*/
bool stringIsEmpty(char* s){
    int i;
    if(!s)
        return true;
    for(i=0;i<strlen(s);i++)
        if(!isspace(s[i]))
            return false;
    return true;
}

/*debugging*/
void printMemPic(int mem[MEMORY_SIZE],int size, const char* headline){
    int i;
    printf("\t--%s memory map--\n",headline);
    for(i=0;i<MEMORY_SIZE && i<size;i++)
            printf("\t\t%d | %d\n",i,mem[i]);
    printf("\t----------------------\n\n");
}

/*calculate how many words this line of original code is gonna take evantually in memory, used by pass1*/
int calcL(char line[LINE_LENGTH], bool startWLable, bool* errorFlag, int lineCounter){
    int i,cur_op=0;
    char *temp = NULL;
    char* token = NULL;
    token = strtok(line, " \t");   /*initialize line to be tokened*/
    clearString(token);
    if(startWLable)
        token = strtok(NULL," \t");   /*skip the lable to start with op*/        
    clearString(token);
    for(i=0;i<OP_COUNT;i++)    /*find the opcode field*/
        if(!strcmp(token,ops[i]))
            cur_op=i;    
    if(cur_op>=14)  /*no paramters- 1 line*/
        return 1;
    if(cur_op==4||cur_op==5||cur_op==7||cur_op==8||cur_op==11||cur_op==12)
        return 2;   /*only 1 paramater*/
    if(cur_op==6)
        return 3;   /*2 parameters*/
    if(cur_op<=3){  /*ops 0/1/2/3*/
        token = strtok(NULL,",");   /*get paramter*/
        if(token2op(token) == REGISTER){   /*if the parameter is register*/
            if(!regIsCorrect(token)){
                printf("ERROR: (Line #%d) reg:[%s] is not legal!\n",lineCounter,token);
                *errorFlag=true;
            }
            token = strtok(NULL,","); /*get next parameter*/            
            if(token2op(token) == REGISTER){
                if(!regIsCorrect(token)){
                printf("ERROR: (Line #%d) reg:[%s] is not legal!\n",lineCounter,token);
                *errorFlag=true;
                }
                return 2;   /*both parameters registers -2 lines*/
            }
        }
        return 3;   /*at least 1 parameter isn't reg- 3 lines*/
    }
    /*left: ops 9,10,13*/
    token = strtok(NULL, "(");  /*token = lable*/
    temp = strtok(NULL, ",");  /*temp = 1st parameter*/
    token = strtok(NULL, ")");  /*token = 2nd parameter*/
    if(!temp)  /*no parameters besides the lable*/
        return 2;   /*1 line only- for lable */
    if(token2op(temp)==REGISTER && token2op(token)==REGISTER){ /*both parameters are registers*/
        if(!regIsCorrect(token)){
                printf("ERROR: (Line #%d) reg:[%s] is not legal!\n",lineCounter,token);
                *errorFlag=true;
        }
        if(!regIsCorrect(temp)){
                printf("ERROR: (Line #%d) reg:[%s] is not legal!\n",lineCounter,temp);
                *errorFlag=true;
        }
        return 3;
    }
    return 4; /*at least one parameter isn't reg then 2 last will take 2 more lines*/

}
/*returns true if token stands for legall register number*/
bool regIsCorrect(char token[LINE_LENGTH]){
    if(token[0]=='r'&&token[1]>='0'&&token[1]<='7' &&! isStringCont(token,2))
       return true;
    return false;
}

/*return: 0-immidiate 1-lable 3-register*/
int token2op(char token[LINE_LENGTH]){
    clearString(token);
    printf("token2op: token-%s ",token);
    if(token[0]=='r'&&token[1]>='0'&&token[1]<='7'){
            printf("to REGISTER\n");
            return REGISTER;
    }
    if(token[0]=='#'&& ++token && atoi(token)  /*what if #0?*/){
        printf("to IMMIDIATE\n");
        return IMMIDIATE;
    }
    printf("to LABLE\n");
    return LABLE;
}

/*converte word from opword structure to int*/
int opWord2int(OpWord* word){
    return word->ERA+ 4*word->dst+ 16*word->src+ 64*word->opcode+ 1024*word->par2+ 4096*word->par1;
}

/*returns true if the string s continues (with non whitespaces) after index 'from'*/
bool isStringCont(char* s, int from){
    int i;
    if(!s)  /*on null string*/
        return false;
    for(i=from;i<strlen(s);i++){
        if(isalnum(s[i])){
            /*printf("string continues: %s\n",s);*/
            return true;
        }
    }
    /*printf("string from[%d] not continues: %s\n",from,s);*/
    return false;
}

/*writes the special binary of the word to output file .ob by int representation*/
void decode_binary(int num, FILE* fp){
    unsigned int mask = 1u << 13;  /* create a mask with the leftmost bit set*/
    int i;
    for(i=0;i<14;i++){
        if (num & mask) 
            fprintf(fp,"/");    /*1*/
        else
            fprintf(fp,".");    /*0*/
        mask >>= 1;  /* shift the mask to the right*/
    }
    fprintf(fp,"\n");
}

/*creates .ent file named filename.ent from the rellevant symbolchart*/
void createEntFile(char* filename, symbolChart* chart){
    int i,len=0,c=0,size = chart->size;
    char name[LINE_LENGTH]="";
    FILE* fp;
    if(!getChartSize(chart))
        return;
    for(i=0;i<size;i++)
        if(chart->linesArray[i]->attributes[entry])
            c++;    /*count how many entries, if none don't open a new file*/
    if(!c)
        return;
    len=strlen(filename);
    strcpy(name,filename);
    name[len-2]='e';
    name[len-1]='n';
    name[len]='t';
    name[len+1]='\0';
    fp = fopen(name,"w");    
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    for(i=0;i<size;i++)
        if(chart->linesArray[i]->attributes[entry])
            fprintf(fp,"%s %d\n",chart->linesArray[i]->symbol, (chart->linesArray[i]->value)+FIRST_MEM_CELL );
    fclose(fp);
}

/*creates .ext file named filename.ext from the rellevant list of externals*/
void createExtFile(char* filename, LinkedList* extApperance){
    int len=0;
    char name[LINE_LENGTH]="";
    FILE* fp;
    ListNode* ptr;
    len=strlen(filename);
    strcpy(name,filename);
    name[len-2]='e';
    name[len-1]='x';
    name[len]='t';
    name[len+1]='\0';
    if(listIsEmpty(extApperance))
        return;
    fp = fopen(name,"w");    
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    ptr = extApperance->head;
    while(ptr){
        fprintf(fp,"%s: %d\n",ptr->symbol,ptr->lineNum);
        ptr = ptr->next;
    }
    fclose(fp);
}

/*creates .ob  file named filename.ob from the memory picture built along the parsing*/
void createObFile(char* filename, int codeMemSize,int dataMemSize,int codeMem[MEMORY_SIZE], int dataMem[MEMORY_SIZE]){
    int i;
    char name[LINE_LENGTH]="";
    FILE* fp;
    strcpy(name,filename);
    name[strlen(filename)-2]='o';
    name[strlen(filename)-1]='b';
    fp = fopen(name,"w");    
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    fprintf(fp,"%d %d\n",codeMemSize,dataMemSize);
    for(i=0;i<codeMemSize;i++){
        fprintf(fp,"%d:",i+FIRST_MEM_CELL);
        decode_binary(codeMem[i],fp);
    }
    for(i=0;i<dataMemSize;i++){
        fprintf(fp,"%d:",FIRST_MEM_CELL + codeMemSize++);
        decode_binary(dataMem[i],fp);
    }
    fclose(fp);
}

/*returns true if error in symbol announcment*/
bool isSymbolError(char* symbol){
    int i;
    for(i=0;i<strlen(symbol);i++)   /*check if lable includes non alfanumberic*/
        if(!isalnum(symbol[i]))            
            return true;        
    for(i=0;i<OP_COUNT;i++)         /*check if lable is name of op*/
        if(!strcmp(symbol,ops[i]))
            return true;
    for(i=0;i<NUM_OF_REGS;i++)      /*check if lable is register*/
        if(symbol[0]=='r' && isStringCont(symbol,1) && atoi(symbol+1)>=0 && atoi(symbol+1)<=NUM_OF_REGS)
            return true;
    return false;
}

/*strips tabs and spaces from given string to singular space for each bulk*/
void strip_extra_spaces(char* str) {
  int i, x;
  for(i=x=0; str[i]; ++i)
    if(!isspace(str[i]) || (i > 0 && !isspace(str[i-1])))
      str[x++] = str[i];
  str[x] = '\0';  
}

/*for debugging*/
void print_binary(int num){
    unsigned int mask = 1u << 13;  /* create a mask with the leftmost bit set*/
    int i;
    for(i=0;i<14;i++){
        if (num & mask) 
            printf("/");    /*1*/
        else
            printf(".");    /*0*/
        mask >>= 1;  /* shift the mask to the right*/
    }
    printf("\n");
}

/*returns true if string is OK*/
bool checkString(char s[LINE_LENGTH]){    
    if(!s)
        return false;
    clearString(s);
    if(s[0]=='\"' && s[strlen(s)-1]=='\"')
        return true;
    return false;
}

/*returns true of data given is legal*/
bool checkData(char token[LINE_LENGTH]){
    while(!stringIsEmpty(token)){            
        if(checkNum(atof(token))){
            token = strtok(NULL,", \t");
        }else 
            return false;
    }
    return true;
}

/*returns true if number is integer*/
bool checkNum(double num){
    return (num == ((int)(num)));
}
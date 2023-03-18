#include "util.h"

char ops[OP_COUNT][OP_MAX_LENGTH]={{"mov"},{"cmp"},{"add"},{"sub"},{"not"},{"clr"},{"lea"},{"inc"},{"dec"},{"jmp"},{"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"}};

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
    /*for(i=0;i<MEMORY_SIZE;i++)
        printf("\t\t%d | %d\n",i,mem[i]);*/
    printf("\t----------------------\n\n");
}

int calcL(char line[LINE_LENGTH], bool startWLable){
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
            token = strtok(NULL,","); /*get next parameter*/
            if(token2op(token) == REGISTER)
                return 2;   /*both parameters registers -2 lines*/
        }
        return 3;   /*at least 1 parameter isn't reg- 3 lines*/
    }
    /*left: ops 9,10,13*/
    token = strtok(NULL, "(");  /*token = lable*/
    temp = strtok(NULL, ",");  /*temp = 1st parameter*/
    token = strtok(NULL, ")");  /*token = 2nd parameter*/
    if(!temp)  /*no parameters besides the lable*/
        return 2;   /*1 line only- for lable */
    if(token2op(temp)==REGISTER && token2op(token)==REGISTER) /*both parameters are registers*/
        return 3;
    return 4; /*at least one parameter isn't reg then 2 last will take 2 more lines*/

}

/*return: 0-immidiate 1-lable 3-register*/
int token2op(char token[LINE_LENGTH]){
    if(token[0]=='r'&&token[1]>='0'&&token[1]<='7')
        return REGISTER;
    if(token[0]=='#'&& ++token && atoi(token)  /*what if #0?*/)
        return IMMIDIATE;
    return LABLE;
}

int opWord2int(OpWord* word){
    return word->ERA+ 4*word->dst+ 16*word->src+ 64*word->opcode+ 1024*word->par2+ 4096*word->par1;
}

bool isStringCont(char* s, int from){
    int i;
    if(!s)  /*on null string*/
        return false;
    for(i=from;i<strlen(s);i++){
        if(isalnum(s[i])){
            printf("string continues: %s\n",s);
            return true;
        }
    }
    printf("string from[%d] not continues: %s\n",from,s);
    return false;
}

void print_binary(int num) {
    unsigned int mask = 1u << (sizeof(int) * 8 - 1);  /* create a mask with the leftmost bit set*/
    while (mask > 0) {
        if (num & mask) {
            printf("1");
        } else {
            printf("0");
        }
        mask >>= 1;  /*shift the mask to the right*/
    }
    printf("\n");
}
#include "util.h"

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

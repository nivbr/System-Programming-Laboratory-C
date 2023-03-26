#include "data.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h> 
#define WORD_FORMAT " %80s"
#define DOUBLE_WORD_FORMAT " %80s %80s"

bool prePro(char** files, int filesCounter);    /*main pre-processing*/
bool doFile(char* filename, hashTable* table);              /*pre processing per file*/
void changeFileType(char* filename);                        /*changes file type from .as to .am*/
char* pushToMacroVal(char* cur, char* new);                 /*add chars into the string which holds macro value*/

/*does the preoricessing, returns true if all went OK*/
bool prePro(char** files, int filesCounter){
    int i;
    hashTable table;
    printf(">start pre pro\n");
    for(i=0;i<filesCounter;i++){
        if(doFile(files[i], &table)){ /*update file names from x.as to x.am */            
            changeFileType(files[i]);
        }
        else
            memset( files[i],0,sizeof(files[i]) );    /*if the file should't be created then delete it's name*/        
    }
    printf(">end pre pro\n");
    return true;
}

/*returns true if file needs to be created*/
bool doFile(char* filename, hashTable* table){  
    bool inMacro = false;
    char* newFileName=NULL;
    char tempMacroName [LINE_LENGTH] = {0};
    char* tempMacroValue = NULL;
    char token[LINE_LENGTH]= ""; /*holds current word*/
    char line[LINE_LENGTH]= "";  /*holds current line*/
    FILE* readFP, *writeFP; /*file pointer to read/write .as/.am */
    hashNode* tempMacroNode = NULL;    
    if((readFP = fopen(filename,"r"))==NULL)        
        return false;   /* ^ file doesn't exist*/    
    /*file do exist-> create new file with .am type*/
    newFileName = (char*)malloc(sizeof(char)*strlen(filename));
    if(!newFileName){        
        fprintf(stdout,"Alocation error ! \n");        
        fclose(readFP);
        exit(1);
    }    
    strcpy(newFileName,filename);
    changeFileType(newFileName);
    printf("newFileName: %s\n",newFileName);    
    writeFP = fopen(newFileName,"w+");     /*open 'after macro file' for writing*/    
    newTable(table);
    while(fgets(line,LINE_LENGTH,readFP)){
        sscanf(line,WORD_FORMAT,token);
        if(strcmp(token,"mcr")==0){   /*if macro defined at this line*/
            inMacro = true;         
            sscanf(line,DOUBLE_WORD_FORMAT,token,tempMacroName); /*load macro name to temp*/
        }
        else if(inMacro){   /*are we inside macro definition ?*/
            if(strstr(line,"endmcr")){  /*end of macro definition*/
                insertItem(tempMacroName,tempMacroValue,table);
                if(tempMacroValue){            
                    free(tempMacroValue);
                    tempMacroValue = NULL;            
                }                    
                inMacro = false;   
            }   
            else /*line is part of macro value*/
                tempMacroValue = pushToMacroVal(tempMacroValue,line);  
            
        }else{  /*not inside macro def*/
            if( (tempMacroNode = searchItem(token,table)) && (strcmp(token,tempMacroNode->key)==0) ) {   /*if current word is macro name*/                       
                fputs(tempMacroNode->value,writeFP); /*write macro value into new file*/
                memset(token, 0, LINE_LENGTH);
            }
            else    /*current word isn't a macro name*/
                fputs(line,writeFP);    /*put line as is*/
        }
    }
    /*finish routine*/
    deleteTable(table);
    fclose(readFP);
    fclose(writeFP);
    free(newFileName);
    return true;
}
/*changes file type from .as to .am*/
void changeFileType(char* filename){
    int length = strlen(filename);
    filename[length-3]='.';
    filename[length-2]='a';
    filename[length-1]='m';
}

/*add chars into the string which holds macro value*/
char * pushToMacroVal(char* cur, char* new){
    if(!new)
        return cur;
    if(!cur)
        cur = (char*)calloc(strlen(new),sizeof(char));
    else
        cur = (char*)realloc( cur,sizeof(char)*( strlen(cur)+strlen(new) ) );
    strcat(cur,new);
    return cur;
}

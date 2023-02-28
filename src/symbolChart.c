#include "symbolChart.h"

    /*private functions prototypes*/
void printLine(Line* line);      /*helps printchart()*/
void copyArr(bool from[4], bool to[4]);     /*obviuos*/
void printAtributtes(bool arr[4]);          /*helps printline()*/

void newSymbolChart(symbolChart* chart){    /*Constructor func*/    
    chart->linesArray = (Line**)calloc(INIT_LINES_CNT,sizeof(Line*)); /*fill all fields*/
    if(chart->linesArray==NULL){
        printf("Alocation Error !\n");
        deleteSymbolChart(chart);
        exit(0);
    }
    chart->size=0;
    chart->maxSize=INIT_LINES_CNT;  /*initial minimal size, doubles up every time getting full*/
}

void deleteSymbolChart(symbolChart* chart){ /*distructor func*/
    Line** lnptr=chart->linesArray;
    int i;
    for(i=0;i<chart->size;i++)
        free( lnptr[i] );
    free(lnptr);
}
    
    /*Insert new row to chart*/
void insertSymbol(Line* line, symbolChart* chart){    
    printf("\nInsert SYMBOL: %s \n\n",line->symbol);
    if((chart->size) >= chart->maxSize){     /*if current array is full -> double it's size up*/
        chart->linesArray = (Line**)realloc(chart->linesArray,(sizeof(Line*)*(chart->maxSize*=2)) );
        if(chart->linesArray==NULL){
            printf("Alocation Error !\n");
            deleteSymbolChart(chart);
            exit(0);
        }
    }
    chart->linesArray[chart->size] = line;  /*connect row to table*/
    chart->size++;  /*increrase size because new row have enterd*/
}
    /*search symbol in table -> return it's row*/
Line* searchSymbol(symbolChart* chart, char* symbol){   /*search symbol in table -> return it's row*/
    int i;
    for(i=0;i<chart->size;i++)
        if(strcmp(chart->linesArray[i]->symbol,symbol)==0)
            return chart->linesArray[i];
    printf("search failed ! \n\t loop ran : %d times \n",i+1);
    return NULL;
}

void printSymbolChart(symbolChart* chart){ 
    int i;
    printf("\n>Symbol Chart: \n");
    for(i=0;i<(chart->size);i++)
        printLine(chart->linesArray[i]);
    printf("\t----------------------------------------------------------------------------------------------------\n");
    printf(">End of Symbol Chart \n\n");
}

void printLine(Line* line){ 
    if(!line){
        printf("Line is eempty !\n");
        return;
    }
    printf("\t----------------------------------------------------------------------------------------------------\n");
    printf("\t|Symbol: %s |",line->symbol);
    printf("|Value: %d |",line->value);
    printf("|Base address: %d |",line->baseAdrs);
    printf("|Offset: %d |",line->offset);
    printAtributtes(line->attributes);
    printf("\n");
}

Line* newSymbol(char* symbol,int value, int baseAdrs, int offset, bool attribute[4]){
    Line* line = (Line *)malloc(sizeof(Line));
    if (!line){
        printf("alocation error !\n");
        exit(0);
    }
    strcpy(line->symbol,symbol);
    line->value = value;
    line->offset = offset;
    line->baseAdrs = baseAdrs;
    copyArr(attribute,line->attributes);
    printf("\nNEW SYMBOL  \n\n");
    return line;
}

void freeSymbol(Line * line){
    free(line->symbol);
    free(line);
    printf("\nFREE SYMBOL  \n\n");
}

void copyArr(bool from[4], bool to[4]){
    int i;
    for(i=0;i<4;i++)
        to[i]=from[i];
}

void printAtributtes(bool arr[4]){  
    int i;
    printf("|Atributtes: ");
    for(i=0;i<4;i++)
        switch (i){
            case external:
            if(arr[i])
                printf("external ");
            break;

            case entry:
            if(arr[i])
                printf("entry ");
            break;
            
            case code:
            if(arr[i])
                printf("code ");
            break;

            case data:
            if(arr[i])
                printf("data ");
            break;

            default:
                printf("\n\nmistake at atributtes ! \n\n");
            break;
        }
}
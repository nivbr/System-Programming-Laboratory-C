#include "symbolChart.h"

    /*private functions prototypes*/
void printLine(Line* line);      /*helps printchart()*/
void printAtributtes(bool arr[4]);          /*helps printline()*/

symbolChart* newSymbolChart(){    /*Constructor func*/
    symbolChart* chart = (symbolChart *)malloc(sizeof(symbolChart));    
    chart->linesArray = (Line**)calloc(INIT_LINES_CNT,sizeof(Line*)); /*fill all fields*/
    if(chart->linesArray==NULL){
        printf("Alocation Error !\n");
        deleteSymbolChart(chart);
        exit(0);
    }
    chart->size=0;
    chart->maxSize=INIT_LINES_CNT;  /*initial minimal size, doubles up every time getting full*/
    return chart;
}

void deleteSymbolChart(symbolChart* chart){ /*distructor func*/
    int i;
    Line** lnptr;
    if (!chart)        
        return;
    lnptr=chart->linesArray;
    for(i=0;i<chart->size;i++)
        free(lnptr[i]);

    free(lnptr);
    free(chart);
}

    /*Insert new row to chart*/
void insertSymbol(Line* line, symbolChart* chart){    
    if((chart->size) >= chart->maxSize){     /*if current array is full -> double it's size up*/
        chart->maxSize*=2;
        chart->linesArray = (Line**)realloc(chart->linesArray,(sizeof(Line*)*(chart->maxSize)) );
        if(chart->linesArray==NULL){
            printf("Alocation Error !(insert symbol)\n");
            deleteSymbolChart(chart);
            exit(0);
        }
    }
    chart->linesArray[chart->size] = line;  /*connect new row to table*/
    chart->size++;  /*increrase size because new row have enterd*/
    printf("\tAdded new symbol [%s]\n",line->symbol);
}
    /*search symbol in table -> return it's row*/
Line* searchSymbol(symbolChart* chart, char* symbol){   /*search symbol in table -> return it's row*/
    int i;
    for(i=0;i<chart->size;i++)
        if(strcmp(chart->linesArray[i]->symbol,symbol)==0)
            return chart->linesArray[i];
    return NULL;
}

void printSymbolChart(symbolChart* chart){ 
    int i;
    if(!chart || !(chart->size) )   /*if chart is empty - don't print*/
        return;
    printf("\n\t>Symbol Chart: \n");
    printf("\t\tchart size: %d\n",chart->size);
    for(i=0;i<(chart->size);i++)
        printLine(chart->linesArray[i]);
    printf("\t\t----------------------------------------------------------------------------------------------------\n");
    printf("\t>End of Symbol Chart \n\n");
}

void printLine(Line* line){ 
    if(!line){
        printf("\t\tLine is eempty !\n");
        return;
    }
    printf("\t\t----------------------------------------------------------------------------------------------------\n");
    printf("\t\t|Symbol: %s |",line->symbol);
    printf("|value: %d |",line->value);
    printf("|Base address: %d |",line->baseAdrs);
    printf("|Offset: %d |",line->offset);
    printAtributtes(line->attributes);
    printf("\n");
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

int getChartSize(symbolChart* chart){
    if(!chart){
        printf("ERROR: chart is empty!");
        return 0;
    }
    return chart->size;
}
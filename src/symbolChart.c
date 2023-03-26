#include "symbolChart.h"

    /*private functions prototypes*/
void printLine(Line* line);      /*helps printchart()*/
void printAtributtes(bool arr[4]);          /*helps printline()*/

symbolChart* newSymbolChart(){    /*Constructor func*/
    symbolChart* chart = (symbolChart *)malloc(sizeof(symbolChart));    
    chart->linesArray = (Line**)calloc(INIT_LINES_CNT,sizeof(Line*)); /*fill all fields*/
    if(chart->linesArray==NULL){        
        fprintf(stdout,"Alocation Error !\n");        
        deleteSymbolChart(chart);
        exit(1);
    }
    chart->size=0;
    chart->maxSize=INIT_LINES_CNT;  /*initial minimal size, doubles up every time getting full*/
    return chart;
}

 /*distructor func*/
void deleteSymbolChart(symbolChart* chart){
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
            fprintf(stdout,"Alocation Error !(insert symbol)\n");            
            deleteSymbolChart(chart);
            exit(0);
        }
    }
    chart->linesArray[chart->size] = line;  /*connect new row to table*/
    chart->size++;  /*increrase size because new row have enterd*/
}

/*search symbol in table -> return it's row*/
Line* searchSymbol(symbolChart* chart, char* lookup){   /*search symbol in table -> return it's row*/
    int i;
    for(i=0;i<chart->size;i++)
        if(!strcmp(((chart->linesArray[i])->symbol),lookup) )
            return chart->linesArray[i];                             
    return NULL;
}

/*debug print the whole chart*/
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

/*debug print line*/
void printLine(Line* line){ 
    if(!line){
        printf("\t\tLine is empty !\n");
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

/*debug print attributes*/
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

/*returns chart size*/
int getChartSize(symbolChart* chart){
    if(!chart){        
        fprintf(stdout,"ERROR: chart is empty!\n");        
        return 0;
    }
    return chart->size;
}

/*updates data base adress by the end of code memory picture*/
void updateDataByIC(symbolChart* chart, int IC){
    int i;
    for(i=0;i<getChartSize(chart);i++)
        if(chart->linesArray[i]->attributes[data])
            (chart->linesArray[i]->value)+=IC;
    
}
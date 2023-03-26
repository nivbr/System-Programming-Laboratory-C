#include "symbolChart.h"    

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
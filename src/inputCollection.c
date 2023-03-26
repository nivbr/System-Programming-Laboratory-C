#include <string.h>
#include <stdlib.h>
#include "data.h"

char** inpCol(char** filesNames, int filesCounter) {
    int i,suffix_length = strlen(".as");
    char** results = malloc(filesCounter * sizeof(char*));
    if (results == NULL) {
        fprintf(stdout,"Error: Memory allocation failed\n");
        exit(1);
    }
    for (i=0;i<filesCounter;i++) {
        int length = strlen(filesNames[i]);
        results[i] = malloc(length + suffix_length + 1);
        if (results[i] == NULL) {
            fprintf(stdout,"Error: Memory allocation failed\n");
            exit(1);
        }
        strcpy(results[i], filesNames[i+1]);
        strcat(results[i], ".as");
    }
    return results;
}
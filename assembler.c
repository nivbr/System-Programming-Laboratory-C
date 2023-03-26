#include "./src/data.h"
#include <stdlib.h>

bool inpCol(int argc, char** argv, char** filesList[4],int howManyFiles[4]);
bool prePro(char**  filesLists[4], int howManyFiles[4]);
bool pass(char** filesList[4],int listCounters[4]);
void distract(char** filesList[4],int ListCounters[4]);

/*main assembler function to be called to start the program*/
int main(int argc, char **argv){

	char** filesLists[4]={0};
	int ListCounters[4]={0};
	
	printf("\n\n----START Program----\n\n");
	inpCol(argc, argv ,filesLists ,ListCounters);
	prePro(filesLists, ListCounters);
	pass(filesLists, ListCounters);
	distract(filesLists, ListCounters);
	printf("\n\n----FINISH Program----\n\n");
	return true;
}

/*free all file name strings*/
void distract(char** filesList[4],int ListCounters[4]){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<ListCounters[i];j++)
			free(filesList[i][j]);
		free(filesList[i]);
	}	
}

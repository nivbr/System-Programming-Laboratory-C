#include "./src/data.h"
#include <stdlib.h>

bool inpCol(int argc, char** argv, char** filesList[4],int howManyFiles[4]);
bool prePro(char**  filesLists[4], int howManyFiles[4]);
/*bool parse(char** filesList[4],int ListCounters[4]);*/
void distract(char** filesList[4],int ListCounters[4]);

int main(int argc, char **argv){

	char** filesLists[4]={0};
	int ListCounters[4]={0};
	
	printf("\n\n----START Program----\n\n");
	inpCol(argc, argv ,filesLists ,ListCounters);
	prePro(filesLists, ListCounters);
	/*parse(filesLists, ListCounters);*/
	distract(filesLists, ListCounters);
	printf("\n\n----FINISH Program----\n\n");
	return true;
}

void distract(char** filesList[4],int ListCounters[4]){
	int i,j;
	printf("---Start Distraction--- \n");
	for(i=0;i<4;i++){
		for(j=0;j<ListCounters[i];j++){
			printf("	>free %s \n",filesList[i][j]);
			free(filesList[i][j]);
		}
		free(filesList[i]);
	}	
	printf("---Finish Distraction--- \n");
}

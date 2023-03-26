#include "./src/data.h"
#include <stdlib.h>

char** inpCol(char** filesNames, int filesCounter);
bool prePro(char** files, int filesCounter);
bool pass(char** files, int filesCounter);
void distract(char** files, int filesCounter);

int main(int argc, char **argv){

	char **files = NULL;
	int filesCounter = argc-1;
	
	printf("\n\n----START Program----\n\n");
	files = inpCol(argv,filesCounter);
	prePro(files, filesCounter);
	pass(files,filesCounter);
	distract(files,filesCounter);
	printf("\n\n----FINISH Program----\n\n");
	return true;
}

void distract(char** files, int filesCounter){
	int i;
	for(i=0;i<filesCounter;i++)
		free(files[i]); 
	free(files);
}
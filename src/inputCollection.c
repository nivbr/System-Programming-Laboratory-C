#include <string.h>
#include <stdlib.h>
#include "data.h"

bool inpCol(int argc, char** argv, char** filesList[4],int howManyFiles[4]){
	int i;
	printf(">Input collection \n");	
	howManyFiles[original]=argc-1;
	filesList[original] = (char**)malloc((argc-1)*sizeof(char*)); /*argc-1 beacuse program name doesnt count*/
	if (!filesList[original]){
		printf("Memory alocation failed !\n");
		exit(1);
	}
	for(i=0;i<howManyFiles[original];i++){	/*add file types to file names */							 
		filesList[original][i] = (char*)malloc((strlen(argv[i])+3)*sizeof(char));	/*add the original string to the newly empty string*/
		strcpy(filesList[original][i], argv[i+1]); /*add at it's last 3 chars the .as file type finish*/
		strcat(filesList[original][i], ".as");	
		if(!filesList[original][i]){
			printf("Alocation error! \n");
			exit(1);
		}
	} 
	printf(">End of Input collection \n\n");	
	return true;
}
#include "hash.h"
#include "data.h"

/*private functions*/
int hash(const char* key);
void freeNode(hashNode* node);
void printHashLine(hashNode* node);
void deleteRaw(hashNode* head);

int hash(const char* key) {	/*hash function returns sum of two first char ascii % HASH_KEY_SIZE*/
   int ret=1,i;
	if(key==NULL){
		printf("Oops your'e looking for an empty string! \n");			
		exit(0);
	}
   for(i=0;i<2 && i<strlen(key);i++)
		ret*=(int)(key[i]);
	return (unsigned)(ret%HASH_KEY_SIZE);
}

hashNode *searchItem(const char * key, hashTable *table) {
	hashNode* node = table->hashArray[hash(key)];
	while(node!=NULL){
		if(!strcmp(node->key,key))
			return node;
		if(node->next==NULL)
			return NULL;
		node = node->next;
	}
	return NULL;
}

void insertItem(const char* key ,const char* value, hashTable *table) {	
	int hashIndex = hash(key);
    hashNode* node = (hashNode *)malloc(sizeof(hashNode)); /*alocate space for node itself*/

	if (node==NULL){
		printf("Aloocation Error ! - add free \n");
		deleteTable(table);
		exit(0);
	}
	/*fill key fields*/
    node->key = (char*)malloc((sizeof(char))*(strlen(key) ) );		/*? (char*)malloc((sizeof(char))*(strlen(key)+1 ) )?*/
	if(node->key == NULL){
		printf("Aloocation Error ! - add free \n");
		free(node);
		deleteTable(table);
		exit(0);
	}
	strcpy(node->key, key);
	/*fill value field*/
	node->value = (char*)malloc((sizeof(char))*(strlen(value) ));	
	if(node->value == NULL){
		printf("Aloocation Error ! - add free \n");
		free(node);
		deleteTable(table);
		exit(0);
	}
	strcpy(node->value, value);
	/*fill next field*/
	node->next=NULL;

	/*push to beggining of the list*/
	if(table->hashArray[hashIndex]!=NULL)
		node->next = table->hashArray[hashIndex];
	table->hashArray[hashIndex] =node;

}

void deleteItem(const char* key, hashTable *table) {
   int hashIndex = hash(key), count=0;
   hashNode* node = table->hashArray[hashIndex];
   hashNode* prev = node;
   
	while(node!=NULL){
		if(strcmp(node->key,key)==0){	/*node found*/
			if(count==0)	/*first node*/
				table->hashArray[hashIndex]=node->next;
			else	/*beyond the first node*/
				prev->next = node->next;
			freeNode(node);
			return;
		}
		prev = node;		/*proceed*/
		node = node->next;
		count++;
	}     
}

void freeNode(hashNode* node){
	free(node->key);
	free(node->value);
	node->next = NULL;
	free(node);
}

void printTable(hashTable *table) {
   int i;
   printf("--HASH--\n");
   for(i = 0; i<HASH_KEY_SIZE; i++) 
	   printHashLine(table->hashArray[i]);
	printf("--END OF HASH--\n");
}

void printHashLine(hashNode* node){
	bool lineEmpty = true;
	while(node!=NULL){
		lineEmpty = false;
		printf("(%s -> %s)",node->key,node->value);
		if(node->next!=NULL)
			printf(", ");
		node = node->next;
	}
	if(!lineEmpty)
		printf("\n");
}

void deleteTable(hashTable *table){
	int i;
	for(i=0;i<HASH_KEY_SIZE;i++){
		deleteRaw(table->hashArray[i]);
		table->hashArray[i]=NULL;
	}
}

void deleteRaw(hashNode* head){
	if(head==NULL)
		return;		
	if(head->next != NULL)
		deleteRaw(head->next);
	freeNode(head);
}

void newTable(hashTable* table){
	int i;
	for(i=0;i<HASH_KEY_SIZE;i++)
		table->hashArray[i]=NULL;
}
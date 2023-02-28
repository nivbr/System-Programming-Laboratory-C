#ifndef HASH    /*dont double define this header*/
#define HASH 

#include <string.h>
#include <stdlib.h>


#define HASH_KEY_SIZE 1000

typedef  struct HashNode {
   char* key;   
   char* value;
   struct HashNode* next;
} hashNode;

typedef struct hashTable{
    hashNode* hashArray[HASH_KEY_SIZE];
} hashTable;

void newTable(hashTable* table);
void insertItem(const char* key,const char* value, hashTable *table);	
void deleteItem(const char* key, hashTable *table);	
hashNode *searchItem(const char * key, hashTable *table);	
void printTable(hashTable *table);	
void deleteTable(hashTable *table);


#endif
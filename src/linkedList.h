#ifndef LINKEDLIST
#define LINKEDLIST

#include "data.h"
#include <stdlib.h>
#include <string.h>

typedef struct ListNode{
    char symbol[LINE_LENGTH];
    int lineNum;
    struct ListNode* next;
}ListNode;

typedef struct LinkedList{
    ListNode* head;
}LinkedList;

LinkedList* newList();
void addToList(char name[LINE_LENGTH], int line, LinkedList* list);
void deleteList(LinkedList* list);
void printLinkedList(LinkedList* list);


#endif
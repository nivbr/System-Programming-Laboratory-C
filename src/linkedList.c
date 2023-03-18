#include "linkedList.h"

LinkedList* newList(){
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if(!list){
        printf("Alocation Error !\n");
        exit(1);
    }
    list->head=NULL;
    return list;
}

void addToList(char name[LINE_LENGTH], int line, LinkedList* list){
    ListNode * node = (ListNode*)malloc(sizeof(ListNode)); 
    ListNode * temp = NULL;
    if(!node){
        printf("Alocation Error !\n");
        exit(1);
    }
    if(!list){
        printf("Error! no such list !\n");
        return;
    }
    if(list->head==NULL)
        list->head=node;
    else{
        temp = list->head;
        list->head = node;
        node->next = temp;
    }
    node->lineNum=line;
    strcpy(node->symbol,name);
}

void deleteList(LinkedList* list){
    ListNode * ptr = list->head, * prev;
    while(ptr){
        prev=ptr;
        ptr=ptr->next;
        free(prev);
    }
    free(list);
}

void printLinkedList(LinkedList* list){
    ListNode* ptr;
    if(!list)   /*empty list- no externals*/
        return;
    ptr = list->head;
    printf("externals:\n");
    while(ptr){
        printf("\t%s %d\n",ptr->symbol,ptr->lineNum);
        ptr = ptr->next;
    }
        
}
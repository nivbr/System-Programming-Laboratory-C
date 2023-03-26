#include "linkedList.h"

/*create new list*/
LinkedList* newList(){
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if(!list){
        printf("Alocation Error !\n");
        exit(1);
    }
    list->head=NULL;
    return list;
}

/*add entry 'name' the appears in line 'line' to the given list*/
void addToList(char name[LINE_LENGTH], int line, LinkedList* list){
    ListNode * node = (ListNode*)malloc(sizeof(ListNode)); 
    ListNode * temp = NULL;
    if(!node){    
        fprintf(stdout,"Alocation Error !\n");        
        exit(1);
    }
    if(!list){        
        fprintf(stdout,"Error! no such list !\n");        
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

/*free list from memory*/
void deleteList(LinkedList* list){
    ListNode * ptr = list->head, * prev;
    while(ptr){
        prev=ptr;
        ptr=ptr->next;
        free(prev);
    }
    free(list);
}

/*print the list*/
void printLinkedList(LinkedList* list){
    ListNode* ptr;
    if(!list)   /*empty list- no externals*/
        return;
    ptr = list->head;
    while(ptr){
        printf("\t\t%s %d\n",ptr->symbol,ptr->lineNum);
        ptr = ptr->next;
    }
        
}

/*returns true if the list is empty*/
bool listIsEmpty(LinkedList* list){
    return !list->head;
}
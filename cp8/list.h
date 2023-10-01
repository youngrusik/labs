#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <stdbool.h>
#include "machine_word.h"

typedef struct node {
    machineWord data;
    struct node* next;
} node;

void destroy(node* head);
void printList(node* head);
void deleteBack(node** head);
void deleteFront(node** head);
void pushBack(node** head, machineWord data);
void pushFront(node** head, machineWord data);
void deleteIndex(node** head, int index);
void pushIndex(node** head, machineWord data, int index);
int sizeList(node* head);

#endif //__LIST_H__
#ifndef __STACK_H__ 
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _stack_node {
    int data;
    struct _stack_node* prev;
} StackNode;

typedef struct _stack {
    StackNode* top;
    size_t size;
} Stack;

void stackInit(Stack* s);
int stackIsEmpty(Stack* s);
size_t stackSize(Stack* s);
void stackPush(Stack* s, int data);
void stackPop(Stack* s);
int stackTop(Stack* s);
void stackRemove(Stack* s);
void stackPrint(Stack* s);

void stackInsertSort(Stack* s);

#endif //__STACK_H__
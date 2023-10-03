#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

void stackInit(Stack* s) {
    s->top = NULL;
    s->size = 0;
}

int stackIsEmpty(Stack* s) {
    return s->size == 0;
}

size_t stackSize(Stack* s) {
    return s->size;
}

void stackPush(Stack* s, int data) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    newNode->data = data;
    newNode->prev = s->top;
    s->top = newNode;
    s->size++;
}

void stackPop(Stack* s) {
    if (stackIsEmpty(s)) {
        printf("Stack is empty\n");
        return;
    }

    StackNode* temp = s->top;
    s->top = s->top->prev;
    free(temp);
    s->size--;
}

int stackTop(Stack* s) {
    if (stackIsEmpty(s)) {
        printf("Stack is empty\n");
        return -1;
    }

    return s->top->data;
}

void stackRemove(Stack* s) {
    while (!stackIsEmpty(s)) {
        stackPop(s);
    }
}

void stackPrint(Stack* s) {
    printf("Stack content: ");
    StackNode* current = s->top;

    while (current != NULL) {
        printf("%d\t", current->data);
        current = current->prev;
    }

    printf("\n");
}

void searchAndMove(Stack* s) {
    if (stackIsEmpty(s) || s->size == 1) {
        return; // Стек пуст или содержит только один элемент, ничего не делаем
    }

    Stack tempStack;
    stackInit(&tempStack);

    int first = stackTop(s);
    stackPop(s);
    stackPush(&tempStack, first);

    while (!stackIsEmpty(s)) {
        int current = stackTop(s);
        int prevTop = stackTop(&tempStack);

        if (current < prevTop) {
            stackPush(&tempStack, current);
            stackPop(s);
        } else 
            break;
        
    }

    while (!stackIsEmpty(&tempStack)) {
        int temp = stackTop(&tempStack);
        stackPop(&tempStack);
        stackPush(s, temp);
    }
}


void stackInsertSort(Stack* s) {
    Stack tempStack;
    stackInit(&tempStack);

    while (!stackIsEmpty(s)) {
        searchAndMove(&tempStack);

        int current = stackTop(s);
        stackPop(s);

        while (!stackIsEmpty(&tempStack) && current < stackTop(&tempStack)) {
            stackPush(s, stackTop(&tempStack));
            stackPop(&tempStack);
        }

        stackPush(&tempStack, current);
    }

    while (!stackIsEmpty(&tempStack)) {
        stackPush(s, stackTop(&tempStack));
        stackPop(&tempStack);
    }
}

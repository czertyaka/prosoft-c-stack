#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACKS 10

typedef int hstack_t;
int numberOfStacks = 0;

struct stack* stackList[MAX_STACKS] = {NULL}; 

struct stackNode
{
    struct stackNode* prev;
    unsigned int size;
    char data[0];
};

struct stack
{
    struct stackNode* head;
    unsigned int numberOfNodes;
    hstack_t handler;
};

hstack_t stack_new(void)
{
    struct stack* stack = malloc(sizeof(struct stack));
    if (stack != NULL)
    {
        stack->head = NULL;
        stack->numberOfNodes = 0u;
        stack->handler = ++numberOfStacks;
        stackList[stack->handler - 1] = stack;
        return stack->handler;
    }
    return -1;
}

void stack_free(const hstack_t hstack)
{
    if(!stack_valid_handler(hstack))
    {
        struct stackNode* node = stackList[hstack - 1]->head;
        while(node != NULL)
        {
            struct stackNode* next = node->prev;
            free(node);
            node = next;
        }
        free(stackList[hstack - 1]);
        stackList[hstack - 1] = NULL;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    if((hstack > 0 && hstack <= numberOfStacks) && (stackList[hstack - 1] != NULL))
    {
        return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    if(!stack_valid_handler(hstack))
    {
        return stackList[hstack - 1]->numberOfNodes;
    }
    return 0u;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(!stack_valid_handler(hstack) && data_in != NULL && size > 0)
    {
        struct stackNode* node = malloc(sizeof(struct stackNode) + size);
        if(node != NULL)
        {
            if(stackList[hstack - 1]->numberOfNodes > 0) {
                node->prev = stackList[hstack - 1]->head;
            }
            else { node->prev = NULL; }
            stackList[hstack - 1]->head = node;
            stackList[hstack - 1]->numberOfNodes++;
            node->size = size;
            memcpy(node->data, data_in, size);
        }
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if(!stack_valid_handler(hstack))
    {
        struct stackNode* node = stackList[hstack - 1]->head;
        if(node != NULL 
            && data_out != NULL 
            && size > 0
            && size >= node->size) 
        {
            memcpy(data_out, node->data, size);
            stackList[hstack - 1]->head = node->prev;
            stackList[hstack - 1]->numberOfNodes--;
            free(node);
            return size;
        }
    }
    return 0u;
}


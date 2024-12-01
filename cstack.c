#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>


#define STACK_LIST_SIZE 30

struct node {
    unsigned int size;
    char *data;
    struct node *prev;
};

typedef struct node node_t;

void node_free(node_t* node) {
    node_t* next_node;
    while(node) {
        next_node = node->prev;
        node->prev = NULL;
        if(node->data) {
            free(node->data);
            node->data = NULL;
        }
        free(node);
        node = next_node;
    }
}

struct stack {
    unsigned int size;
    node_t *head;
};

typedef struct stack stack_t;

struct stack_list {
    int size;
    stack_t* stack[STACK_LIST_SIZE];
} stack_list = {.size = STACK_LIST_SIZE};

hstack_t stack_new(void)
{
    for(hstack_t hstack = 0; hstack < stack_list.size; hstack++) {
        if(stack_list.stack[hstack] == NULL) {
            stack_list.stack[hstack] = malloc(sizeof(stack_t));
            if(stack_list.stack[hstack] == NULL) return -1;
            stack_list.stack[hstack]->size = 0;
            stack_list.stack[hstack]->head = NULL;
            return hstack;
        }
    }
    return -1;
}

int stack_valid_handler(const hstack_t hstack)
{
    if((hstack > -1) && (hstack < stack_list.size)) {
        return !stack_list.stack[hstack];
    }
    return 1;
}


void stack_free(const hstack_t hstack)
{
    if(!stack_valid_handler(hstack)) {
        if(stack_list.stack[hstack]->head) {
            node_free(stack_list.stack[hstack]->head);
        }
        free(stack_list.stack[hstack]);
        stack_list.stack[hstack] = NULL;
    }
}

unsigned int stack_size(const hstack_t hstack)
{
    if(!stack_valid_handler(hstack)) {
        return stack_list.stack[hstack]->size;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(!data_in || !size) return;
    if(stack_valid_handler(hstack)) return;
    node_t *node = malloc(sizeof(node_t));
    if(!node) return;
    stack_list.stack[hstack]->size++;
    node->prev = stack_list.stack[hstack]->head;
    node->size = size;
    node->data = malloc(size);
    char* data = (char*)data_in;
    for(unsigned int i = 0; i < size; i++) {
        node->data[i] = data[i];
    }
    stack_list.stack[hstack]->head = node;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if(!data_out || !size) return 0u;
    if(stack_valid_handler(hstack)) {
        return 0u;
    }
    if(!stack_list.stack[hstack]->head) return 0u;
    if(stack_list.stack[hstack]->head->size != size) return 0u;
    node_t* node = stack_list.stack[hstack]->head;
    if(!node) return 0u;
    stack_list.stack[hstack]->size--;
    stack_list.stack[hstack]->head = node->prev;
    node->prev = NULL;
    char* data = (char*)data_out;
    unsigned int i = 0;
    for(; i < size; i++) {
        data[i] = node->data[i];
    }
    node_free(node);
    return i;
}


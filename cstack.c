#include "cstack.h"
#include <stddef.h>

struct node
{
    struct node* next;
    const unsigned int size;
    void* data;
};

typedef struct node node_t;
typedef node_t* stack_t;

void* stack_new()
{
    return NULL;
}

void stack_free(void*)
{}

unsigned int stack_size(void*)
{
    return 0;
}

void stack_push(void*, const void*, const unsigned int)
{}

unsigned int stack_pop(void*, void*, const unsigned int)
{
    return 0;
}


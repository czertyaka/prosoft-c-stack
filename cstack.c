#include "cstack.h"
#include <stddef.h>

struct node
{
    const struct node* prev;
    unsigned int size;
    void* data;
};

typedef struct node node_t;
typedef const node_t* stack_t;

hstack_t stack_new()
{
    return -1;
}

void stack_free(const hstack_t)
{}

int stack_valid_handler(const hstack_t)
{
    return 1;
}

unsigned int stack_size(const hstack_t)
{
    return 0;
}

void stack_push(const hstack_t, const void*, const unsigned int)
{}

unsigned int stack_pop(const hstack_t, void*, const unsigned int)
{
    return 0;
}


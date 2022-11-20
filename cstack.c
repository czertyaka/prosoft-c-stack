#include "cstack.h"
#include <stddef.h>

struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
};

typedef struct node node_t;
typedef const node_t* stack_t;

hstack_t stack_new()
{
    return -1;
}

void stack_free(const hstack_t hstack)
{
    (void)hstack;
}

int stack_valid_handler(const hstack_t hstack)
{
    (void)hstack;
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    (void)hstack;
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    (void)hstack;
    (void)data_in;
    (void)size;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    (void)hstack;
    (void)data_out;
    (void)size;
    return 0;
}


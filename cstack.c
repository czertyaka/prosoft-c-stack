#include "cstack.h"
#include <stddef.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
};

struct stack_entry
{
    int reserved;
    stack_t stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};

struct node* front_node [5];



hstack_t stack_new()
{
    
    return -1;
}


//////////////////

void stack_free(const hstack_t hstack)
{
    UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{
    UNUSED(hstack);
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    UNUSED(hstack);
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_in);
    UNUSED(size);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_out);
    UNUSED(size);
    return 0;
}


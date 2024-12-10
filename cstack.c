#include "cstack.h"
#include <stddef.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)
#define INITIAL_SIZE 10

typedef struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
} node;

typedef struct node* stack_t;

struct stack_entry
{
    int reserved;
    unsigned int stack_size;
    stack_t stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};


hstack_t stack_new(void)
{
    return -1;
}

void stack_free(const hstack_t hstack)
{
    UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{   // Returns: 0 - соответствующий хэндлеру стек существует, 1 - нет.
    if (hstack >= 0 && hstack < g_table.size && g_table.entries[hstack].reserved){
        return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{   // Returns: количество элементов в стеке, или 0 в противном случае.
    if(stack_valid_handler(hstack)){
        return g_table.entries[hstack].stack_size;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(!stack_valid_handler(hstack) || !data_in || !size){
        return;
    }

    stack_t stack = g_table.entries[hstack].stack;

    node* new_node = (node*)malloc(sizeof(node) + size);
    if(!new_node){
        return;
    }

    new_node->prev = stack;
    new_node->size = size;
    memcpy(new_node->data, data_in, size);

    g_table.entries[hstack].stack = new_node;
    g_table.entries[hstack].stack_size++;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{   // Returns: кол-во байт записанных данных, или 0 в противном случае.
    if(!stack_valid_handler(hstack) || !data_out || !size){
        return 0;
    }
    if(!g_table.entries[hstack].stack_size){
        return 0;
    }

    stack_t stack = g_table.entries[hstack].stack;
    node* current_node = stack;
    if(!stack || stack->size > size){
        return 0;
    }
    unsigned int data_size = stack->size;

    memcpy(data_out, stack->data, size);
    g_table.entries[hstack].stack = current_node->prev;
    g_table.entries[hstack].stack_size--;
    free(current_node);
    return data_size;
}

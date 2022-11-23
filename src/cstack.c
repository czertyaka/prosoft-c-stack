#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(VAR) (void)(VAR)

typedef struct node
{
    struct node* prev;
    unsigned int size;
    const void* data[0];
}node_t; 

typedef struct stack_entry
{
    int reserved;
    node_t* top_stack;
}stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};

hstack_t stack_new()
{
    if (g_table.entries == NULL)
        g_table.entries = malloc(sizeof(g_table.entries));
    node_t* top = malloc(sizeof(top));
    int handler = -1;
    for (int i = 0; i < g_table.size; i++) 
    {
        if (g_table.entries[i].reserved == 0)
            handler = i;
    }
    if (handler == -1)
        handler = g_table.size;
    g_table.entries[handler].top_stack = top;
    g_table.entries[handler].reserved = 1;
    g_table.entries[handler].top_stack->prev = NULL;
    g_table.size += 1;
    return handler;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 1)
        return;
    g_table.entries[hstack].reserved = 0;
    node_t* ptr = g_table.entries[hstack].top_stack;
    while(ptr != NULL) 
    {
        g_table.entries[hstack].top_stack->prev = g_table.entries[hstack].top_stack;
        free(g_table.entries[hstack].top_stack);
    }
    UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{
    return hstack < g_table.size && g_table.entries[hstack].reserved == 1 ? 0 : 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 1)
        return 0;
    node_t* ptr = g_table.entries[hstack].top_stack;
    int size = 0;
    if (g_table.entries[hstack].top_stack->data == NULL)
        return 0;
    while(ptr != NULL) 
    {
        size += 1;
        ptr = g_table.entries[hstack].top_stack->prev;
    }
    return size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack) == 1)
        return;
    if (data_in == NULL) return;
    node_t* ptr = g_table.entries[hstack].top_stack;
    g_table.entries[hstack].top_stack = malloc(sizeof(node_t));
    g_table.entries[hstack].top_stack->prev = ptr;
    g_table.entries[hstack].top_stack->data[0] = data_in;// указатель на первый байт 
    g_table.entries[hstack].top_stack->size = sizeof(data_in) + sizeof(g_table.entries[hstack].top_stack->prev) 
    + g_table.entries[hstack].top_stack->size;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (stack_valid_handler(hstack) == 1)
        return 0;
    if (size >= sizeof(g_table.entries[hstack].top_stack->data))
        data_out = g_table.entries[hstack].top_stack->data;
    else for (int i = 0; i < sizeof(g_table.entries[hstack].top_stack->data); i++) 
    {
       // data_out[i] = g_table.entries[hstack].stack.top->data[i];
    }
    node_t* ptr = g_table.entries[hstack].top_stack;
    while(ptr != NULL) 
    {
        g_table.entries[hstack].top_stack = g_table.entries[hstack].top_stack->prev;
        ptr = g_table.entries[hstack].top_stack->prev;
    }
    return sizeof(data_out);
}
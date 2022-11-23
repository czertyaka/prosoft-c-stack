#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
}; 


typedef struct node node_t;

struct stack 
{
    hstack_t hstack;
    node_t* top;
};

typedef struct stack stack_t;


struct stack_entry
{
    int reserved;// bool
    stack_t* stack;
};

typedef struct stack_entry stack_entry_t;


struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};

hstack_t stack_new()
{
    g_table.entries = malloc(sizeof(g_table.entries));
    node_t* top = malloc(sizeof(top));
    g_table.entries[g_table.size].stack.hstack = g_table.size;
    g_table.entries[g_table.size].stack.top = top;
    g_table.entries[g_table.size].reserved = sizeof(stack_t);
    g_table.entries[g_table.size].stack.top->prev = NULL;
    g_table.size += 1;
    return g_table.entries[g_table.size - 1].stack.hstack;
}

void stack_free(const hstack_t hstack)
{
    UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{
    for (int i = 0; i < g_table.size; i++) 
    {
        if (g_table.entries[i].stack.hstack == hstack)
            return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    node_t* ptr = g_table.entries[hstack].stack.top;
    int size = 0;
    while(ptr != NULL) 
    {
        size += 1;
        g_table.entries[hstack].stack.top = g_table.entries[hstack].stack.top->prev;
        ptr = g_table.entries[hstack].stack.top->prev;
    }
    return size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (data_in == NULL) return;
    node_t* ptr = g_table.entries[hstack].stack.top;
    g_table.entries[hstack].stack.top = malloc(sizeof(node_t));// 
    g_table.entries[hstack].stack.top->prev = ptr;
    g_table.entries[hstack].stack.top->data[0] = data_in;// указатель на первый байт 
    g_table.entries[hstack].stack.top->size = sizeof(data_in) + sizeof(g_table.entries[hstack].stack.top->prev) 
    + g_table.entries[hstack].stack.top->size;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (size >= sizeof(g_table.entries[hstack].stack.top->data))
        data_out = g_table.entries[hstack].stack.top->data;
    else for (int i = 0; i < sizeof(g_table.entries[hstack].stack.top->data); i++) 
    {
       // data_out[i] = g_table.entries[hstack].stack.top->data[i];
    }
    node_t* ptr = g_table.entries[hstack].stack.top;
    while(ptr != NULL) 
    {
        g_table.entries[hstack].stack.top = g_table.entries[hstack].stack.top->prev;
        ptr = g_table.entries[hstack].stack.top->prev;
    }
    return sizeof(data_out);
}


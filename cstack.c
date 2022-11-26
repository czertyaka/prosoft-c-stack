#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

typedef struct node
{
    const struct node* prev;
    unsigned int size;
    const void* data;
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
    if (g_table.entries == NULL) {
        g_table.entries = (stack_entry_t*)malloc(sizeof(stack_entry_t));
        if (g_table.entries == NULL) exit(1);
    }

    int handler = -1;
    for (int i = 0; i < (int)g_table.size; i++) 
    {
        if (g_table.entries[i].reserved == 0)
            handler = i;
    }

    if (handler == -1) {
        handler = g_table.size;
        g_table.entries = (stack_entry_t*)realloc(g_table.entries, (sizeof(stack_entry_t)) * (g_table.size + 1));
        if (g_table.entries == NULL) exit(1);
    }
        
    g_table.entries[handler].top_stack = NULL;
    g_table.entries[handler].reserved = 1;
    g_table.size++;
    return handler;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 1)
        return;

    const node_t* ptr = g_table.entries[hstack].top_stack;
    const node_t* prev;

    while(ptr != NULL) 
    {
    	free((void*)ptr->data);
        prev = ptr->prev;
        free((void*)ptr);
        ptr = prev;
    }

    g_table.entries[hstack].reserved = 0;
    g_table.size--;
    
    if (g_table.size == 0) {
        free(g_table.entries);
        g_table.entries = NULL;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    return g_table.entries == NULL || hstack >= (int)g_table.size || g_table.entries[hstack].reserved == 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 1)
        return 0u;
    
    const node_t* ptr = g_table.entries[hstack].top_stack;
    unsigned int size = 0u;
    while(ptr != NULL) 
    {
        size += 1;
        ptr = ptr->prev;
    }
    return size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack) == 1)
        return;
    if (data_in == NULL || size == 0u) return;

    node_t* ptr_top = g_table.entries[hstack].top_stack;
    node_t* ptr_new_top = (node_t*)malloc(sizeof(node_t) + sizeof(ptr_top->data));
    ptr_new_top->prev = ptr_top;
    
    void* data = (void*)malloc(size);
    memcpy (data, data_in, size);
    ptr_new_top->data = data;
    ptr_new_top->size = size + sizeof(ptr_new_top->prev) + sizeof(size);

    g_table.entries[hstack].top_stack = ptr_new_top;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (size == 0u || stack_valid_handler(hstack) == 1 || data_out == NULL || g_table.entries[hstack].top_stack == NULL)
        return 0u;

    unsigned int size_data_out = 0u;
    node_t* ptr_top = g_table.entries[hstack].top_stack;
    unsigned int size_data_in_top = ptr_top->size - sizeof(ptr_top->prev) - sizeof(ptr_top->size);

    if (size < size_data_in_top) 
       return 0u;

    size_data_out = size;
    memcpy(data_out, ptr_top->data, size_data_in_top);

    const node_t* ptr_new_top = ptr_top->prev;
    
    free((void*)ptr_top->data);
    free((void*)ptr_top);
    g_table.entries[hstack].top_stack = (node_t*)ptr_new_top;
    return size_data_out;
}
#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int H_newStack = -1;

    if (g_table.entries == NULL) 
    {
       g_table.entries = (stack_entry_t*)malloc(sizeof(stack_entry_t));
       if (g_table.entries == NULL) 
            return -1;

       g_table.size++;
    }

    memset(g_table.entries,0,sizeof(stack_entry_t));

    for (int i = 0; i < (int)g_table.size; i++) 
    {
        if (g_table.entries[i].reserved == 0)
            {
                g_table.entries[i].top_stack = NULL;
                g_table.entries[i].reserved = 1;
                H_newStack = i;
            }            
    }

    if (H_newStack == -1)
    {
        H_newStack = g_table.size;
        g_table.entries = (stack_entry_t*)realloc(g_table.entries, (sizeof(stack_entry_t)) * (g_table.size + 1));
    }

    if (g_table.entries == NULL)
    {
        return -1;
    }

    return H_newStack;
    
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 1)
        return;

    const node_t* p_stack = g_table.entries[hstack].top_stack;
    const node_t* prev_stack;

    while(p_stack != NULL) 
    {
    	free((void*)p_stack->data);
        prev_stack = p_stack->prev;
        free((void*)p_stack);
        p_stack = prev_stack;
    }

    g_table.entries[hstack].reserved = 0;
    g_table.size--;
    
    if (g_table.size == 0) 
    {
        free(g_table.entries);
        g_table.entries = NULL;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    if (g_table.entries == NULL || hstack < 0 || hstack >= (int)g_table.size || g_table.entries[hstack].reserved == 0)
        return 1;
    else 
        return 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 1)
        return 0;

    const node_t* p_stack = g_table.entries[hstack].top_stack;
    unsigned int stack_size = 0;
    while(p_stack != NULL) 
    {
        stack_size++;
        p_stack = p_stack->prev;
    }
    return stack_size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack) == 1 || data_in == NULL || size == 0)
    {
        return;
    }

    node_t* Top_Stack = g_table.entries[hstack].top_stack;
    node_t* New_Top_Stack = (node_t*) malloc(sizeof(node_t) + sizeof(Top_Stack->size));
    
    if (New_Top_Stack == NULL)
    {
        return;
    }

    New_Top_Stack->size = size;

    void* data = (void*)malloc(size);
    
    memcpy(data,data_in,size);
    New_Top_Stack->data = data;

    New_Top_Stack->prev = Top_Stack;
    g_table.entries[hstack].top_stack = New_Top_Stack;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (stack_valid_handler(hstack) == 1 || data_out == NULL || size == 0 || g_table.entries[hstack].top_stack == NULL || size < g_table.entries[hstack].top_stack->size)
    {
        return 0;
    }

   node_t* Pointer_Top_Stack = g_table.entries[hstack].top_stack;
   g_table.entries[hstack].top_stack = (node_t*)Pointer_Top_Stack->prev;

   memcpy(data_out,Pointer_Top_Stack->data,size);
   int result = size;

   free((node_t*)Pointer_Top_Stack->data);
   free((node_t*)Pointer_Top_Stack);

   return result;
}


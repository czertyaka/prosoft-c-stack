#include "cstack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
}node_t;

typedef struct stack_entry
{
    int reserved;
    node_t* stack;
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
    {
    	g_table.entries = malloc(sizeof(stack_entry_t));
    	if (g_table.entries == NULL)
    		return -1;	
	}
	int handler = -1;
	for (hstack_t i = 0; i < g_table.size; i++)
		if (g_table.entries[i].reserved == 0)
			handler = i;	
	if (handler == -1)
	{
		handler = g_table.size;
		g_table.entries = realloc(g_table.entries, (g_table.size + 1) * sizeof(stack_entry_t));
		if (g_table.entries == NULL)
			return -1;
	}	
	g_table.size++;
	g_table.entries[handler] = (stack_entry_t){1u, NULL};
	return handler;	
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 0)
    {
        node_t* node = g_table.entries[hstack].stack;
		while(node != NULL)
		{
			node_t* prev = node -> prev;
        	free((void*)node);
			node = prev;
		}	
		g_table.entries[hstack].reserved = 0;
    	g_table.size--;
    }  
}

int stack_valid_handler(const hstack_t hstack)
{
    return hstack < 0 || (unsigned int)hstack >= g_table.size|| g_table.entries[hstack].reserved == 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 1)
        return 0u;
    node_t* node = g_table.entries[hstack].stack;
    unsigned int size = 0u;
    while(node != NULL) 
    {
        size++;
        node = node->prev;
    }
    return size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if ((stack_valid_handler(hstack) == 0) && (data_in != NULL) && (size > 0))
    {
        node_t *new_top = malloc(sizeof(node_t) + sizeof(char) * size);
        if (new_top == NULL)
            return;
        new_top->size = size;
        new_top->prev = g_table.entries[hstack].stack;
        memcpy(new_top->data, data_in, size);
        g_table.entries[hstack].stack = new_top;
    }

}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if ((stack_valid_handler(hstack) == 0) && (size >= g_table.entries[hstack].stack->size) && (data_out != NULL))
    {
        node_t *top = g_table.entries[hstack].stack;
        node_t *prev = top->prev;
        g_table.entries[hstack].stack = prev;
        unsigned int new_size = top->size;
        memcpy(data_out, top->data, new_size);
        free(top);
        return new_size;
    }
    return 0;
}

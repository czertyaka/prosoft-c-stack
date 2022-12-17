#include "cstack.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct node
{
    struct node* prev;
    unsigned int size;
    void* data;
}node_t;
 
typedef struct stack_entry
{
    bool reserved;
    node_t* top;
}stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0, NULL};

hstack_t stack_new()
{
    unsigned int handler = g_table.size;
	
	if(g_table.entries == NULL)
	{
	    g_table.entries = (stack_entry_t*)malloc(sizeof(stack_entry_t));
		g_table.size++;
		if (g_table.entries == NULL)
        return -1;
	}
    else
	{
		for (unsigned int i = 0; i < g_table.size; i++)
		{
			if(!g_table.entries[i].reserved)
			    handler = i;
		}
		    if (handler == g_table.size)
			{
				g_table.entries = (stack_entry_t*)realloc(g_table.entries, (g_table.size+1) * sizeof(stack_entry_t));
				g_table.size++;
				if (g_table.entries == NULL)
                    return -1;
			}
				
	}		
	
    g_table.entries[handler].top = NULL;
    g_table.entries[handler].reserved = true;
    return handler;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
        return;
	
    const node_t* top = g_table.entries[hstack].top;
    g_table.entries[hstack].reserved = false;
	
    while (top != NULL)
    {
        free((void*)top->data);
        const node_t *temp = top -> prev;
        free((void*)top);
        top = temp;
    }
	
    g_table.entries[hstack].top = NULL;
    
    if (!g_table.size) 
    {
        free(g_table.entries);
        g_table.entries = NULL;
    }

}

int stack_valid_handler(const hstack_t hstack)
{
    if ( (int)hstack >= (int)g_table.size || g_table.entries == NULL|| hstack < 0 || g_table.entries[hstack].reserved == false)
	    return 1;
    else return 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
        return 0;

    const node_t *currentFrame = g_table.entries[hstack].top;
    int size = 0;

    for ( ;currentFrame != NULL; size++)
    {
        currentFrame = currentFrame->prev;
    }
    return size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack) || data_in == NULL || size == 0)
	    return ;
	
    node_t *node_prev_top = g_table.entries[hstack].top;
    node_t* node_cur_top = (node_t*)malloc(sizeof(node_t));
	

    if (node_cur_top == NULL)
        return;

    node_cur_top->data = (void*)malloc(size); //неисправленное замечание
	
    if (node_cur_top->data == NULL)
	    return;

    if (g_table.entries[hstack].top == NULL)
        node_cur_top->prev = NULL;

    else node_cur_top->prev = node_prev_top;
	
    memcpy(node_cur_top->data, data_in, size);
    node_cur_top->size = size;
    g_table.entries[hstack].top = node_cur_top;
	//free(node_cur_top);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (stack_valid_handler(hstack) || data_out == NULL || size == 0 || g_table.entries[hstack].top == NULL)
	    return 0;
	
    node_t* node_prev_top = g_table.entries[hstack].top;
    node_t* node_cur_top = g_table.entries[hstack].top->prev;
	
    const unsigned int size_top = node_prev_top->size;
    if (size < size_top) 
       return 0;
   
    const void *data_in = node_prev_top->data;
    memcpy(data_out, data_in, size);
	
    free((void*)data_in);
    node_prev_top->data = NULL;
	
    g_table.entries[hstack].top = node_cur_top;
    free((void*)node_prev_top);
    return size;
}

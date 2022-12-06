#include "cstack.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
    int handler = g_table.size;
    g_table.entries = (stack_entry_t*)realloc(g_table.entries, (g_table.size+1) * sizeof(stack_entry_t));
    if (g_table.entries == NULL)
         return -1;

    g_table.entries[handler].top = NULL;
    g_table.entries[handler].reserved = true;
    g_table.size++;
    return handler;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
        return;
	
    const node_t* top = g_table.entries[hstack].top;
	g_table.entries[hstack].reserved = false;
	
     for (; top != NULL;)
    {
         free((void*)top->data);
         const node_t *temp = top -> prev;
         free((void*)top);
         g_table.entries[hstack].top = NULL;
         top = temp;
    }
	
    g_table.size--;
    
    if (!g_table.size) 
    {
        free(g_table.entries);
        g_table.entries = NULL;
    }
	
}
int stack_valid_handler(const hstack_t hstack)
{
    int handler = hstack;
	if ( hstack >= (int)g_table.size || g_table.entries == NULL|| hstack < 0)
		return 1;
    else if (g_table.entries[handler].reserved == true)
        return 0;
    else return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
        return 0;

    const node_t *currentFrame = g_table.entries[hstack].top;
    int i = 0;

    for ( ;currentFrame != NULL; i++)
    {
        currentFrame = currentFrame->prev;
    }
    return i;
}
void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
	if (stack_valid_handler(hstack) || data_in == NULL || size == 0)
		return ;
	
    node_t *node_prev_top = g_table.entries[hstack].top;
	node_t* node_cur_top = (node_t*)malloc(sizeof(node_t) + sizeof(node_prev_top->data));
	

    if (node_cur_top == NULL)
        return;

    node_cur_top->data = (void*)malloc(size);
	
	if (node_cur_top->data == NULL)
		return;

    if (g_table.entries[hstack].top == NULL)
        node_cur_top->prev = NULL;

    else if (g_table.entries[hstack].top != NULL)
        node_cur_top->prev = node_prev_top;

    memmove(node_cur_top->data, data_in, size);
    node_cur_top->size = sizeof(node_cur_top->size) + sizeof(node_cur_top->prev) + size;
    g_table.entries[hstack].top = node_cur_top;
}
unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
	if (stack_valid_handler(hstack) || data_out == NULL || size == 0 || g_table.entries[hstack].top == NULL)
		return 0;
	
    node_t* node_prev_top = g_table.entries[hstack].top;
    node_t* node_cur_top = g_table.entries[hstack].top->prev;
	
	const unsigned int size_top = node_prev_top->size - sizeof(node_prev_top->prev) - sizeof(node_prev_top->size);
    if (size < size_top) 
       return 0;
   
	const void *data_in = node_prev_top->data;
    memmove(data_out, data_in, size);
	
    free((void*)data_in);
    node_prev_top->data = NULL;
	
    g_table.entries[hstack].top = node_cur_top;
    free((void*)node_prev_top);
    return size;
}

#include "cstack.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

typedef struct node
{
    struct node* prev;
    unsigned int size;
    void *data;
} node_t;

typedef struct stack_entry
{
    int reserved;
    node_t *stack_top;
} stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = { 0u, NULL };

hstack_t stack_new()
{
    int handler = g_table.size;
    if (!g_table.size)
    {
        g_table.entries = (stack_entry_t*)malloc(sizeof(stack_entry_t));
        if (g_table.entries == NULL)
            return -1;
        g_table.entries[handler].stack_top = NULL;
        g_table.entries[handler].reserved = handler;
        g_table.size++;
        return handler;
    }
    if (g_table.size)
    {
        g_table.entries = (stack_entry_t*)realloc(g_table.entries, (g_table.size) * sizeof(stack_entry_t));
        if (g_table.entries == NULL)
            return -1;
        g_table.entries[handler].stack_top = NULL;
        g_table.entries[handler].reserved = handler;
        g_table.size++;
        return handler;
    }
    return 0;
}

void stack_free(const hstack_t hstack)
{
    assert(!stack_valid_handler(hstack));
	
     for (; g_table.entries[hstack].stack_top != NULL;)
    {
         free(g_table.entries[hstack].stack_top->data);
         node_t *temp = g_table.entries[hstack].stack_top -> prev;
         free(g_table.entries[hstack].stack_top);
         g_table.entries[hstack].stack_top = NULL;
         g_table.entries[hstack].stack_top = temp;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    int handler = hstack;
    for (unsigned int i = 0; i < g_table.size; i++)
    if (g_table.entries[i].reserved == handler)
    {
        return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
        assert(!stack_valid_handler(hstack));

    const node_t *currentFrame = g_table.entries[hstack].stack_top;
    int i = 0;

    for ( ;currentFrame != NULL; i++)
    {
        currentFrame = currentFrame->prev;
    }
    return i;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size) ///
{
       assert(!stack_valid_handler(hstack));
       assert(!(data_in == NULL || size == 0));

       node_t *node_prev_top = g_table.entries[hstack].stack_top;
       node_t* node_cur_top = (node_t*)malloc(sizeof(node_t) + sizeof(node_prev_top->data));
       assert(!(node_cur_top == NULL));

    if (g_table.entries[hstack].stack_top == NULL)
    {
        node_cur_top->data = (void*)malloc(size);
        node_cur_top->prev = NULL;
        memmove(node_cur_top->data, data_in, size);
        node_cur_top->size = sizeof(node_cur_top->size) + sizeof(node_cur_top->prev) + sizeof(node_cur_top->data);
        g_table.entries[hstack].stack_top = node_cur_top;
    }
    else if (g_table.entries[hstack].stack_top != NULL)
    {
        node_cur_top->data = (void*)malloc(size);
        node_cur_top->prev = node_prev_top;
        memmove(node_cur_top->data, data_in, size);
        node_cur_top->size = sizeof(node_cur_top->size) + sizeof(node_cur_top->prev) + sizeof(node_cur_top->data);
        g_table.entries[hstack].stack_top = node_cur_top;
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    node_t* node_prev_top = g_table.entries[hstack].stack_top;
    node_t* node_cur_top = g_table.entries[hstack].stack_top->prev;
	const void *data = node_prev_top->data;
    memmove(data_out, data, size);
    free(node_prev_top->data);
    node_prev_top->data = NULL;
    g_table.entries[hstack].stack_top = node_cur_top;
    free(node_prev_top);
    return size;

}


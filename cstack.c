#include "cstack.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
};
typedef struct node node_t;

struct stack_entry
{
    unsigned int free_bytes;
    node_t* stack;
};
typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int count;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = { 0u, NULL };

#define STACK_RESERVED 1024
#define ADDED_STACK_ENTRIES 4

hstack_t alloc_stack(const hstack_t hstack)
{
    node_t* stack = malloc(STACK_RESERVED);
    if (stack == NULL)
    {
        return -1;
    }
    memset(stack, 0, STACK_RESERVED);

    g_table.entries[hstack].free_bytes = STACK_RESERVED;
    g_table.entries[hstack].stack = stack;
    return hstack;
}

hstack_t stack_new()
{
    for (int i = 0; i < g_table.count; ++i)
    {
        if (g_table.entries[i].stack == NULL)
        {
            return alloc_stack(i);
        }
    }

    // no free stack handlers
    unsigned int result = g_table.count;
    stack_entry_t* entries_array = malloc((g_table.count + ADDED_STACK_ENTRIES) * sizeof(stack_entry_t));
    if (entries_array == NULL)
    {
        return -1;
    }

    if (g_table.entries)
    {
        memcpy(entries_array, g_table.entries, g_table.count * sizeof(stack_entry_t));
        free(g_table.entries);
    }
    memset(entries_array + g_table.count, 0, ADDED_STACK_ENTRIES * sizeof(stack_entry_t));
    g_table.entries = entries_array;
    g_table.count += ADDED_STACK_ENTRIES;

    return alloc_stack(result);
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
    {
        assert("Invalid stack handler!");
        return;
    }

    node_t* end = g_table.entries[hstack].stack;
    while (end->prev)
    {
        end = end->prev;
    }
    free(end);
    g_table.entries[hstack].stack = NULL;

    --g_table.count;
    if (g_table.count == 0)
    {
        free(g_table.entries);
        g_table.entries = NULL;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    //0 - stack exist, 1 - not exist
    return hstack < 0 || hstack >= g_table.count || g_table.entries[hstack].stack == NULL;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
    {
        assert("Invalid stack handler!");
        return 0;
    }

    node_t* begin = g_table.entries[hstack].stack;
    unsigned int count = 0;
    while (begin->prev)
    {
        begin = begin->prev;
        ++count;
    }
    return count;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack) || data_in == NULL || size == 0)
    {
        assert("Invalid stack handler!");
        return;
    }
    const unsigned int object_size = size + sizeof(node_t);
    if (g_table.entries[hstack].free_bytes < object_size)
    {
        // TODO выделяем память в стеке, копируем старое
        
    }
    node_t* stack = g_table.entries[hstack].stack;
    stack->size = size;
    memcpy(stack->data, data_in, size);
    char* current = stack;
    current += object_size;
    g_table.entries[hstack].stack = current;
    g_table.entries[hstack].stack->prev = stack;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (stack_valid_handler(hstack) || data_out == NULL || size == 0
        || g_table.entries[hstack].stack->prev == NULL
        || g_table.entries[hstack].stack->prev->size > size)
    {
        assert("Invalid stack handler!");
        return 0;
    }

    node_t* data_node = g_table.entries[hstack].stack->prev;
    memcpy(data_out, data_node->data, data_node->size);
    g_table.entries[hstack].stack = g_table.entries[hstack].stack->prev;
    return data_node->size;
}

#include "cstack.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
} node_t;
#define SIZE_OF_NODE (sizeof(void*) + sizeof(unsigned int))

typedef struct stack_entry
{
    bool bIsReserved;
    node_t* stack;
} stack_entry_t;

struct stack_entries_table
{
    unsigned int count;
    unsigned int free_count;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = { 0u, 0u, NULL };

#define ADDED_STACK_ENTRIES 4

hstack_t stack_new()
{
    if (g_table.free_count != 0)
    {
        for (int i = 0; i < g_table.count; ++i)
        {
            if (!g_table.entries[i].bIsReserved)
            {
                g_table.entries[i].bIsReserved = true;
                return i;
            }
        }
    }
    // no free stack handlers
    const unsigned int result = g_table.count;
    const auto array_size = (g_table.count + ADDED_STACK_ENTRIES) * sizeof(stack_entry_t);
    stack_entry_t* entries_array = malloc(array_size);
    if (entries_array == NULL)
    {
        return -1;
    }
    memset(entries_array, 0, array_size);

    if (g_table.entries)
    {
        memcpy(entries_array, g_table.entries, g_table.count * sizeof(stack_entry_t));
        free(g_table.entries);
    }
    g_table.entries = entries_array;
    g_table.count += ADDED_STACK_ENTRIES;
    g_table.free_count += ADDED_STACK_ENTRIES - 1;

    g_table.entries[result].bIsReserved = true;
    return result;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
    {
        assert("Invalid stack handler!");
        return;
    }

    node_t* end = g_table.entries[hstack].stack;
    if (end == NULL)
    {
        g_table.entries[hstack].bIsReserved = false;
        return;
    }

    while (end->prev)
    {
        node_t* current = end->prev;
        free(end);
        end = current;
    }
    free(end);
    g_table.entries[hstack].bIsReserved = false;
    g_table.entries[hstack].stack = NULL;

    ++g_table.free_count;
    if (g_table.count == g_table.free_count)
    {
        free(g_table.entries);
        g_table.entries = NULL;
        g_table.count = g_table.free_count = 0;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    //0 - stack exist, 1 - not exist
    return hstack < 0 || hstack >= g_table.count || g_table.entries[hstack].bIsReserved == false;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
    {
        assert("Invalid stack handler!");
        return 0;
    }

    node_t* end = g_table.entries[hstack].stack;
    if (end == NULL)
    {
        return 0;
    }

    unsigned int count = 1;
    while (end->prev)
    {
        ++count;
        end = end->prev;
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

    node_t* elem = malloc(size + SIZE_OF_NODE);
    if (elem == NULL)
    {
        return;
    }
    elem->size = size;
    memcpy(elem->data, data_in, size);

    node_t* current = g_table.entries[hstack].stack;
    elem->prev = current;
    g_table.entries[hstack].stack = elem;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (stack_valid_handler(hstack) || data_out == NULL || size == 0
        || g_table.entries[hstack].stack == NULL
        || g_table.entries[hstack].stack->size > size)
    {
        assert("Invalid stack handler!");
        return 0;
    }
    node_t* data = g_table.entries[hstack].stack;
    g_table.entries[hstack].stack = data->prev;

    memcpy(data_out, data->data, data->size);
    const auto result = data->size;
    free(data);
    return result;
}

#include "cstack.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
} node_t;

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

#define CHECK_AFTER_ALLOC(ptr, return_value) if(ptr == NULL) \
                                             { \
                                                 return return_value; \
                                             } else (void)0

hstack_t stack_new()
{
    if (g_table.free_count != 0)
    {
        for (unsigned int i = 0; i < g_table.count; ++i)
        {
            if (!g_table.entries[i].bIsReserved)
            {
                g_table.entries[i].bIsReserved = true;
                --g_table.free_count;
                return i;
            }
        }
    }
    // no free stack handlers
    const unsigned int result = g_table.count;
    const int array_size = (g_table.count + ADDED_STACK_ENTRIES) * sizeof(stack_entry_t);
    stack_entry_t* const entries_array = malloc(array_size);
    CHECK_AFTER_ALLOC(entries_array, -1);

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

void free_table_if_not_needed()
{
    if (g_table.count == g_table.free_count)
    {
        free(g_table.entries);
        g_table.entries = NULL;
        g_table.count = g_table.free_count = 0;
    }
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
    {
        return;
    }

    const node_t* end = g_table.entries[hstack].stack;
    if (end == NULL)
    {
        g_table.entries[hstack].bIsReserved = false;
        ++g_table.free_count;
        free_table_if_not_needed();
        return;
    }

    while (end->prev)
    {
        const node_t* const current = end->prev;
        free((node_t*)end);
        end = current;
    }
    free((node_t*)end);
    g_table.entries[hstack].bIsReserved = false;
    g_table.entries[hstack].stack = NULL;

    ++g_table.free_count;
    free_table_if_not_needed();
}

int stack_valid_handler(const hstack_t hstack)
{
    //0 - stack exist, 1 - not exist
    return hstack < 0 || (unsigned int)hstack >= g_table.count || g_table.entries[hstack].bIsReserved == false;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
    {
        return 0;
    }

    const node_t* end = g_table.entries[hstack].stack;
    unsigned int count = 0;
    while (end)
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
        return;
    }

    node_t* elem = malloc(size + sizeof(node_t));
    CHECK_AFTER_ALLOC(elem, );

    elem->size = size;
    memcpy(elem->data, data_in, size);

    node_t* current = g_table.entries[hstack].stack;
    elem->prev = current;
    g_table.entries[hstack].stack = elem;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (stack_valid_handler(hstack)
        || data_out == NULL
        || size == 0
        || g_table.entries[hstack].stack == NULL
        || g_table.entries[hstack].stack->size > size)
    {
        return 0;
    }
    const node_t* const data = g_table.entries[hstack].stack;
    g_table.entries[hstack].stack = (node_t*)data->prev;

    memcpy(data_out, data->data, data->size);
    const int result = data->size;
    free((node_t*)data);
    return result;
}

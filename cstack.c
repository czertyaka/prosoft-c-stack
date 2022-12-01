#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    struct node* prev;
    unsigned int size;
    char data[0];
};

typedef struct node node_t;

struct stack_entry
{
    unsigned int presence;
    unsigned int size;
    node_t* stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    unsigned int capacity;
    unsigned const int grow_factor; // TODO name
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, 0u, 2u, NULL};

int resize_g_table() {
    stack_entry_t* new_entries = NULL;
    if (g_table.size == 0u) {
        new_entries = calloc(g_table.grow_factor, sizeof(stack_entry_t));
        if(!new_entries)
            return -1;
        g_table.capacity = g_table.grow_factor;
    } else {
        new_entries = calloc(g_table.grow_factor * g_table.capacity, sizeof(stack_entry_t));
        if(!new_entries)
            return -1;
        memcpy(new_entries, g_table.entries, g_table.capacity * sizeof(stack_entry_t));
        g_table.capacity = g_table.grow_factor * g_table.capacity;
        free(g_table.entries);
    }
    g_table.entries = new_entries;
    return 0;
}

hstack_t stack_new()
{
    // TODO search of deleted stacks (private stack?)

    if (g_table.size >= g_table.capacity) {
        int flag = resize_g_table();
        if (flag != 0)
            return -1;
    }
    stack_entry_t new_entry = {1u, 0,NULL};
    g_table.entries[g_table.size] = new_entry;
    g_table.size++;
    return (int) g_table.size - 1;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) != 0) {
        return;
    }
    stack_entry_t* pStackEntry = &g_table.entries[hstack];
    node_t* new_head = pStackEntry->stack;
    node_t* head;
    while(new_head) {
        head = new_head;
        new_head = new_head->prev;
        free(head);
    }
    pStackEntry->presence = 0u;
    pStackEntry->size = 0u;
}

int stack_valid_handler(const hstack_t hstack)
{
    if (hstack >= 0 && hstack < (int)g_table.size && g_table.entries[hstack].presence == 1u) {
        return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 0) {
        return g_table.entries[hstack].size;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (!data_in || size == 0u || stack_valid_handler(hstack) != 0) {
        return;
    }
    struct node* new_node = (struct node *) malloc(sizeof(struct node) + size);
    if (!new_node) {
        return;
    }
    new_node->size = size;
    memcpy(new_node->data, data_in, size);

    stack_entry_t* pStackEntry = &g_table.entries[hstack];
    if (stack_size(hstack) == 0)
    {
        new_node->prev = NULL;
        pStackEntry->stack = new_node;
    } else {
        new_node->prev = pStackEntry->stack;
        pStackEntry->stack = new_node;
    }
    pStackEntry->size++;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (data_out == 0 || size == 0u || stack_valid_handler(hstack) != 0) {
        return 0;
    }
    stack_entry_t* pStackEntry = &g_table.entries[hstack];
    if (pStackEntry->size == 0u) {
        return 0;
    }
    node_t* head = pStackEntry->stack;
    if (head->size <= size) {
        unsigned int result_size = head->size;
        memcpy(data_out, head->data, result_size);
        pStackEntry->stack = head->prev;
        free(head);
        pStackEntry->size--;
        return result_size;
    } else {
        return 0u;
    }
}


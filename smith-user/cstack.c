#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define RESERVED_HANDLER 0
#define GROW_FACTOR 2

struct node {
    const struct node* prev;
    unsigned int size;
    char data[0];
};

typedef struct node node_t;

struct stack_entry {
    unsigned int present;
    unsigned int size;
    node_t* stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table {
    unsigned int size;
    unsigned int capacity;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, 0u, NULL};

int alloc_g_table_entries() {
    g_table.entries = (stack_entry_t*) calloc(GROW_FACTOR, sizeof(stack_entry_t));
    if(!g_table.entries) {
        return -1;
    }
    g_table.capacity = GROW_FACTOR;
    return 0;
}

void init_reserved_stack() {
    stack_entry_t reserved_stack = {1u, 0, NULL};
    g_table.entries[RESERVED_HANDLER] = reserved_stack;
    g_table.size++;
}

int resize_g_table() {
    stack_entry_t* new_entries = (stack_entry_t*) reallocarray(
            g_table.entries,
            GROW_FACTOR * g_table.capacity,
            sizeof(stack_entry_t));
    if(!new_entries) {
        return -1;
    }
    g_table.capacity *= GROW_FACTOR;
    g_table.entries = new_entries;
    return 0;
}

hstack_t stack_new() {
    if(!g_table.entries) {
        if(alloc_g_table_entries() != 0) {
            return -1;
        }
        init_reserved_stack();
    }
    stack_entry_t new_entry = {1u, 0,NULL};
    if(stack_size(RESERVED_HANDLER) != 0) {
        int idx;
        unsigned int result_size = stack_pop(RESERVED_HANDLER, &idx, sizeof(idx));
        if (result_size == sizeof(idx)) {
            g_table.entries[idx] = new_entry;
            return idx;
        }
    }
    if (g_table.size >= g_table.capacity) {
        if (resize_g_table() != 0) {
            return -1;
        }
    }
    g_table.entries[g_table.size] = new_entry;
    g_table.size++;
    return (int) g_table.size - 1;
}

void stack_free(const hstack_t hstack) {
    if (stack_valid_handler(hstack) != 0) {
        return;
    }
    stack_entry_t* pStackEntry = &g_table.entries[hstack];
    const node_t* new_head = pStackEntry->stack;
    const node_t* head;
    while(new_head) {
        head = new_head;
        new_head = new_head->prev;
        free((void *)head);
    }
    stack_push(RESERVED_HANDLER, &hstack, sizeof(hstack));
    pStackEntry->present = 0u;
    pStackEntry->size = 0u;
}

int stack_valid_handler(const hstack_t hstack) {
    return hstack < 0
            || hstack >= (int)g_table.size
            || g_table.entries[hstack].present == 0u
            || hstack == RESERVED_HANDLER;
}

unsigned int stack_size(const hstack_t hstack) {
    return (stack_valid_handler(hstack) == 0) ? g_table.entries[hstack].size : 0u;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size) {
    if (!data_in || size == 0u || stack_valid_handler(hstack) != 0) {
        return;
    }
    node_t* new_node = (node_t *) malloc(sizeof(node_t) + size);
    if (!new_node) {
        return;
    }
    new_node->size = size;
    memcpy(new_node->data, data_in, size);
    stack_entry_t* pStackEntry = &g_table.entries[hstack];
    if (pStackEntry->size == 0u) {
        new_node->prev = NULL;
    } else {
        new_node->prev = pStackEntry->stack;
    }
    pStackEntry->stack = new_node;
    pStackEntry->size++;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size) {
    if (data_out == NULL || size == 0u || stack_valid_handler(hstack) != 0) {
        return 0u;
    }
    stack_entry_t* pStackEntry = &g_table.entries[hstack];
    if (pStackEntry->size == 0u) {
        return 0u;
    }
    node_t* head = pStackEntry->stack;
    if (head->size > size) {
        return 0u;
    }
    unsigned int result_size = head->size;
    memcpy(data_out, head->data, result_size);
    pStackEntry->stack = (node_t *) head->prev;
    free(head);
    pStackEntry->size--;
    return result_size;
}


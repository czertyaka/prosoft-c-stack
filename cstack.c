#include "cstack.h"

// Реализация записи данных позаимствована отсюда
// https://constantsmatter.com/posts/generic-ds-c/

// *** LIBRARY SETTINGS ***

#define STACK_TABLE_HANDLER_MIN_CAP 10
#define STACK_MIN_CAP_SIZE 256
#define UNUSED(VAR) (void)(VAR)

// ========================

#pragma pack(push, 1)
typedef struct node 
{
    unsigned int      size;
    void*                   data;
    struct node*      prev;
} node_t;
#pragma pack(pop)

#pragma pack(push, 1)
struct stack
{
    unsigned int            reserved;
    struct node*            entry;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct stack_entries_table 
{
    int            size_stack;

    int            capacity;
    int            count;
    struct stack*     stacks; // Указывает на указатели входа в стеки
};
#pragma pack(pop)

struct stack_entries_table g_table =
{
    STACK_MIN_CAP_SIZE,
    STACK_TABLE_HANDLER_MIN_CAP,
    0,
    NULL
};

hstack_t stack_new(void)
{
    if (g_table.count == 0)
        stack_init();

    ++g_table.count;
    return 0;
}

void stack_init(void) {
    g_table.stacks = malloc((sizeof(char*) * g_table.capacity) * g_table.size_stack);
    for (int i = 0; i < g_table.capacity; ++i) {
        g_table.stacks[i].entry = malloc(sizeof(node_t) * g_table.size_stack);
    }
}

void stack_free(const hstack_t hstack)
{
    free(&g_table.stacks[hstack]);
}

int stack_valid_handler(const hstack_t hstack)
{
    UNUSED(hstack);
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    UNUSED(hstack);
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    node_t* ptr = malloc(sizeof(node_t) + size);
    if (ptr != NULL) {
        ptr->size = size;
        ptr->data = ptr + sizeof size;
        memcpy(ptr, data_in, size);
        ptr->prev = g_table.stacks[hstack].entry;
        g_table.stacks[hstack].entry = ptr;
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (g_table.stacks[hstack].entry == NULL)
        return 0;
    memcpy(data_out, g_table.stacks[hstack].entry, size);
    node_t* ptr_prev = g_table.stacks[hstack].entry->prev;
    free(g_table.stacks[hstack].entry);
    g_table.stacks[hstack].entry = ptr_prev;
    return size;
}


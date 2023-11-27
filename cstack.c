#include "cstack.h"

// Ðåàëèçàöèÿ çàïèñè äàííûõ ïîçàèìñòâîâàíà îòñþäà
// https://constantsmatter.com/posts/generic-ds-c/

// *** LIBRARY SETTINGS ***

#define STACK_TABLE_HANDLER 12
#define UNUSED(VAR) (void)(VAR)

// ========================

#pragma pack(push, 1)
typedef struct node 
{
    void*                   data;
    struct node*            prev;
} node_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct stack
{
    struct node*            entry;
} stack_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct stack_entries_table 
{
    stack_t*                stacks[STACK_TABLE_HANDLER]; // Óêàçûâàåò íà óêàçàòåëè âõîäà â ñòåêè
    int                     count;
} stack_entries_table_t;
#pragma pack(pop)

stack_entries_table_t g_table = {.stacks = NULL, .count = 0 };

hstack_t stack_new(void)
{
    stack_t* _stack =        (stack_t*)malloc(sizeof(stack_t));

    if (_stack == NULL) {
        return -1;
    }
    _stack->entry = NULL;

    g_table.stacks[g_table.count] = _stack;

    return g_table.count++;
}

void stack_free(const hstack_t hstack)
{
    if (g_table.stacks[hstack] == NULL || 
        hstack >= STACK_TABLE_HANDLER)
    {
        return;
    }

    stack_t* ptrStack = g_table.stacks[hstack];
    if (ptrStack->entry != NULL) {
        free(ptrStack->entry);
    }
    free(ptrStack);
    g_table.stacks[hstack] = (stack_t*)NULL;
    --g_table.count;
}

int stack_valid_handler(const hstack_t hstack)
{
    if (g_table.stacks[hstack] == NULL ||
        hstack < 0 ||
        hstack >= STACK_TABLE_HANDLER) {
        return 1;
    }
    return 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (g_table.stacks[hstack] == NULL || 
        hstack >= STACK_TABLE_HANDLER) 
    {
        return 0;
    }

    int _i = 0;

    for (node_t* _node = g_table.stacks[hstack]->entry; !(_node == NULL); _node = _node->prev) {
        ++_i;
    }
    return _i;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (g_table.stacks[hstack] == NULL || 
        hstack >= STACK_TABLE_HANDLER) 
    {
        return;
    }

    if (data_in == NULL || size <= 0) {
        return;
    }

    node_t* ptr = (node_t*)malloc(sizeof(node_t));
    
    if (ptr == NULL) {
        return;
    }
   

    ptr->data = malloc(size);
    if (ptr->data == NULL) {
        free(ptr);
        return;
    }

    memcpy(ptr->data, data_in, size);

    ptr->prev = g_table.stacks[hstack]->entry;
    
    g_table.stacks[hstack]->entry = ptr;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (g_table.stacks[hstack] == NULL || hstack >= STACK_TABLE_HANDLER) {
        return 0;
    }

    if (data_out == NULL || size <= 0) {
        return 0;
    }

    if (g_table.stacks[hstack]->entry == NULL) {
        return 0;
    }

    memcpy(data_out, g_table.stacks[hstack]->entry->data, size);

    node_t* _nextTopNode = g_table.stacks[hstack]->entry->prev;

    free(g_table.stacks[hstack]->entry->data);
    free(g_table.stacks[hstack]->entry);

    g_table.stacks[hstack]->entry = _nextTopNode;
    return size;
}


#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    struct node *prev;
    unsigned int size;
    char data[0];
} node_t;

typedef struct stack
{
    struct node *top;
    int size;
} stack_t;

typedef struct stack_table
{
    int reserved;
    int last;
    struct stack *pTable;
} stack_table_t;

stack_table_t g_table = {16, -1, NULL};

hstack_t stack_new()
{
    if (g_table.last == -1)
    {
        g_table.pTable = (stack_t *)realloc(g_table.pTable, g_table.reserved * sizeof(stack_t) + 1);
        if (!g_table.pTable)
            return -1;
    }
    else if (g_table.last == (g_table.reserved - 1))
    {
        for (int i = 0; i <= g_table.last; ++i)
        {
            if (g_table.pTable[i].size == -1)
            {
                g_table.pTable[i].size = 0;
                return i;
            }
        }

        g_table.reserved = g_table.reserved * 2;
        g_table.pTable = (stack_t *)realloc(g_table.pTable, g_table.reserved * sizeof(stack_t) + 1);
        
        if (!g_table.pTable)
            return -1;
    }
    ++g_table.last;
    g_table.pTable[g_table.last] = (stack_t){NULL, 0};
    return g_table.last;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 0)
    {
        node_t *current = g_table.pTable[hstack].top;
        if (current)
        {
            node_t *prevPointer = current->prev;
            for (int i = 0; i < g_table.pTable[hstack].size; ++i)
            {
                free(current);
                current = prevPointer;
                if (current)
                    prevPointer = current->prev;
            }
        }
        g_table.pTable[hstack].size = -1;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    if ((hstack >= 0)
        && (hstack <= g_table.last)
        && (g_table.pTable[hstack].size != -1))
        return 0;
    else
        return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) == 0)
        return g_table.pTable[hstack].size;
    else
        return 0;
}

void stack_push(const hstack_t hstack, const void *data_in, const unsigned int size)
{
    if ((stack_valid_handler(hstack) == 0)
        && (data_in)
        && (size > 0))
    {
        node_t *pNewNode = (node_t *)malloc(sizeof(node_t) + sizeof(char) * size);
        if (!pNewNode)
            return;
            
        pNewNode->size = size;
        pNewNode->prev = g_table.pTable[hstack].top;
        memcpy(pNewNode->data, data_in, size);
        g_table.pTable[hstack].top = pNewNode;
        ++g_table.pTable[hstack].size;
    }
}

unsigned int stack_pop(const hstack_t hstack, void *data_out, const unsigned int size)
{
    if ((data_out)
        && (stack_valid_handler(hstack) == 0)
        && (g_table.pTable[hstack].size > 0)
        && (size >= g_table.pTable[hstack].top->size))
    {
        node_t *pTop = g_table.pTable[hstack].top;
        node_t *pPrev = pTop->prev;
        
        g_table.pTable[hstack].top = pPrev;
        --g_table.pTable[hstack].size;
        unsigned int newSize = pTop->size;
        memcpy(data_out, pTop->data, newSize);
        free(pTop);
        return newSize;
    }
    return 0;
}
#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node
{
    struct node *prev;
    unsigned int size;
    char data[0];
} node_t;

typedef struct stack
{
    struct node *top;
    int stackSize;
} stack_t;

typedef struct stack_table
{
    int reservedSize;
    int lastStack;
    struct stack *pTable;
} stack_table_t;

stack_table_t g_table = {.reservedSize = 16, .lastStack = -1, .pTable = NULL};

hstack_t stack_new()
{
    if (g_table.lastStack == NULL)
    {
        g_table.pTable = (stack_t *)malloc(g_table.reservedSize * sizeof(stack_t));
        if (g_table.pTable == NULL)
            return -1;
    }
    else if (g_table.lastStack == (g_table.reservedSize - 1))
    {
        for (int i = 0; i <= g_table.lastStack; i++)
        {
            if (g_table.pTable[i].stackSize == -1)
            {
                g_table.pTable[i].stackSize = 0;
                return i;
            }
        }

        g_table.reservedSize = g_table.reservedSize * 2;
        g_table.pTable = (stack_t *)realloc(g_table.pTable, g_table.reservedSize * sizeof(stack_t));

        if (g_table.pTable == NULL)
            return -1;
    }
    g_table.lastStack++;
    g_table.pTable[g_table.lastStack] = (stack_t){.top = NULL, .stackSize = 0};
    return g_table.lastStack;
}

void stack_free(const hstack_t hstack)
{

}

int stack_valid_handler(const hstack_t hstack)
{

}

unsigned int stack_size(const hstack_t hstack)
{

}

void stack_push(const hstack_t hstack, const void *data_in, const unsigned int size)
{

}

unsigned int stack_pop(const hstack_t hstack, void *data_out, const unsigned int size)
{
    
}
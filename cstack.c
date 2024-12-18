#include "cstack.h"
#include <stddef.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)
#define INITIAL_SIZE 10

typedef struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
} node;

typedef node* stack_t;

struct stack_entry
{
    int reserved;
    unsigned int stack_size;
    stack_t stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};


hstack_t stack_new(void)
{   //Returns: -1 в случае ошибки выполнения, хэндлер нового стека в случае успеха.
    
    if (!g_table.entries)
    {
        // Первоначальное выделение памяти для массива стеков
        g_table.entries = calloc(INITIAL_SIZE, sizeof(stack_entry_t));
        if (!g_table.entries)
        {
            return -1;
        }
        g_table.size = INITIAL_SIZE;
    }

    hstack_t index;
    // Подбор свободного индекса
    for (index; index < g_table.size && g_table.entries[index].reserved; ++index);
    
    if (index >= g_table.size)
    {
        // Расширение массива стеков
        stack_entry_t* new_array = calloc(g_table.size * 2, sizeof(stack_entry_t));
        if (!new_array)
        {
            return -1;
        } 
        // Копируем массив стеков в новую область памяти
        memcpy(new_array, g_table.entries, g_table.size * sizeof(stack_entry_t));
        free(g_table.entries);
        g_table.entries = new_array;
        g_table.size *= 2;        
    } else
    {
        g_table.entries[index].reserved = 1;
        return index;
    }

}

void stack_free(const hstack_t hstack)
{
    if (!stack_valid_handler)
    {
        return;
    }

    stack_entry_t* stack_entry = &g_table.entries[hstack];
    stack_t current_node = stack_entry->stack;

    stack_t prev_node;
    while (current_node)
    {
        prev_node = current_node->prev;
        free(current_node);
        current_node = prev_node;
    }
    stack_entry->stack = NULL;
    stack_entry->reserved = 0;
    stack_entry->stack_size = 0;
    
    // Проверяем остались ли стеки в массиве и  если нет освобождаем память
    for (hstack_t i = 0; i < g_table.size; i++)
    {
        if (g_table.entries[i].reserved)
        {
            return;
        }        
    }
    free(g_table.entries);
    g_table.entries = NULL;
    g_table.size = 0;        
}

int stack_valid_handler(const hstack_t hstack)
{   // Returns: 0 - соответствующий хэндлеру стек существует, 1 - нет.
    if (hstack >= 0 && hstack < g_table.size && g_table.entries[hstack].reserved)
    {
        return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{   // Returns: количество элементов в стеке, или 0 в противном случае.
    if(stack_valid_handler(hstack))
    {
        return g_table.entries[hstack].stack_size;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(!stack_valid_handler(hstack) || !data_in || !size)
    {
        return;
    }

    stack_t stack = g_table.entries[hstack].stack;

    node* new_node = (node*)malloc(sizeof(node) + size);
    if(!new_node)
    {
        return;
    }

    new_node->prev = stack;
    new_node->size = size;
    memcpy(new_node->data, data_in, size);

    g_table.entries[hstack].stack = new_node;
    g_table.entries[hstack].stack_size++;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{   // Returns: кол-во байт записанных данных, или 0 в противном случае.
    if(!stack_valid_handler(hstack) || !data_out || !size)
    {
        return 0;
    }
    if(!g_table.entries[hstack].stack_size)
    {
        return 0;
    }

    stack_t stack = g_table.entries[hstack].stack;
    stack_t current_node = stack;
    unsigned int data_size = stack->size;
    if(!stack || data_size > size)
    {
        return 0;
    }
    
    memcpy(data_out, stack->data, size);
    g_table.entries[hstack].stack = current_node->prev;
    g_table.entries[hstack].stack_size--;
    free(current_node);
    return data_size;
}

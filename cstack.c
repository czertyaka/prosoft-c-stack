#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node 
{
    struct node* prev;  // Указатель на предыдущий элемент
    unsigned int size;  // Размер данных
    char data[];        // Гибкий массив для хранения данных
};

typedef struct node* stack_t;

struct stack_entry
{
    int reserved; // Флаг, показывающий, выделено ли место под стек
    stack_t stack; // Указатель на стек
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;       // Размер таблицы
    stack_entry_t* entries;  // Массив записей о стеках
};

struct stack_entries_table g_table = {0u, NULL};

// Функция для изменения размера таблицы
static int resize_table(unsigned int new_size) 
{
    stack_entry_t* new_entries = (stack_entry_t*)realloc(g_table.entries, new_size * sizeof(stack_entry_t));
    if (new_entries == NULL) 
    {
        return -1; // Ошибка выделения памяти
    }

    // Инициализация новых элементов
    for (unsigned int i = g_table.size; i < new_size; ++i) 
    {
        new_entries[i].reserved = 0;
        new_entries[i].stack = NULL;
    }

    g_table.entries = new_entries;
    g_table.size = new_size;
    return 0;
}

// Функция для поиска свободного места в таблице
static int find_space(void) 
{
    for (unsigned int i = 0; i < g_table.size; ++i) 
    {
        if (g_table.entries[i].reserved == 0) 
        {
            return i; // Найдено свободное место
        }
    }
    return -1; // Нет свободного места
}

// Функция для инициализации таблицы
static int initialize_table(void) 
{
    g_table.entries = (stack_entry_t*)malloc(5 * sizeof(stack_entry_t));
    if (g_table.entries == NULL) 
    {
        return -1; // Ошибка выделения памяти
    }

    g_table.size = 5;

    // Инициализация таблицы
    for (unsigned int i = 0; i < g_table.size; ++i) 
    {
        g_table.entries[i].reserved = 0;
        g_table.entries[i].stack = NULL;
    }

    return 0;
}

// Функция для создания нового стека
hstack_t stack_new(void) 
{
    if (g_table.size == 0) 
    {
        if (initialize_table() == -1) 
        {
            return -1; // Ошибка инициализации
        }
    }

    int handler = find_space();
    if (handler == -1) 
    {
        if (resize_table(g_table.size * 2) == -1) 
        {
            return -1; // Ошибка выделения памяти
        }
        handler = find_space();
    }

    g_table.entries[handler].reserved = 1; // Помечаем место как занятое
    g_table.entries[handler].stack = NULL; // Инициализируем стек как пустой
    return handler;
}

// Функция для освобождения стека
void stack_free(const hstack_t stack) 
{
    if (stack_valid_handler(stack)) 
    {
        return; // Неверный хэндлер стека
    }

    struct node* current = g_table.entries[stack].stack;
    while (current) 
    {
        struct node* temp = current;
        current = current->prev;
        free(temp);
    }

    g_table.entries[stack].reserved = 0; // Помечаем место как свободное
    g_table.entries[stack].stack = NULL;
}

// Функция для проверки валидности хэндлера стека
int stack_valid_handler(const hstack_t stack) 
{
    return (stack < 0 || (unsigned int)stack >= g_table.size || g_table.entries[stack].reserved == 0);
}

// Функция для получения размера стека
unsigned int stack_size(const hstack_t stack) 
{
    if (stack_valid_handler(stack) == 0 && g_table.entries[stack].stack != NULL) 
    {
        int count_obj = 0;
        struct node* current = g_table.entries[stack].stack;
        while (current) 
        {
            current = current->prev;
            count_obj++;
        }
        return count_obj;
    }
    return 0;
}

// Функция для добавления элемента в стек
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size) 
{
    if (stack_valid_handler(stack) != 0 || data_in == NULL || size == 0) 
    {
        return;
    }

    struct node* new_node = (struct node*)malloc(sizeof(struct node) + size);
    if (new_node == NULL) 
    {
        return; // Ошибка выделения памяти
    }

    memcpy(new_node->data, data_in, size);
    new_node->size = size; // Размер данных в байтах
    new_node->prev = g_table.entries[stack].stack;
    g_table.entries[stack].stack = new_node;
}

// Функция для извлечения элемента из стека
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size) 
{
    if (stack_valid_handler(stack) != 0 || g_table.entries[stack].stack == NULL) 
    {
        return 0;
    }

    struct node* top_node = g_table.entries[stack].stack;
    unsigned int copy_size = (size < top_node->size) ? size : top_node->size;

    if (data_out != NULL) 
    {
        memcpy(data_out, top_node->data, copy_size);
    }

    g_table.entries[stack].stack = top_node->prev;
    free(top_node);
    return copy_size;
}

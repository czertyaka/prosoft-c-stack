// cstack.c
#include "cstack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INITIAL_TABLE_SIZE 16U  

typedef struct node {
    struct node* prev;
    unsigned int size; //размер данных в байтах
    char data[];       //гибкий массив с данными
} node_t;

typedef struct {
    int used;        //0 = свободeн, 1 = занят
    node_t* top;     //указатель на вершину стека (NULL если пуст)
    unsigned int count; //количество элементов в стеке
} stack_entry_t;

typedef struct {
    unsigned int size;       //количество слотов в entries
    stack_entry_t* entries;  //динамический массив записей
} stack_table_t;

// глобальная таблица дескрипторов
static stack_table_t g_table = {0u, NULL};

//проверка инициализации таблицы
static int ensure_table_initialized(void)
{
    if (g_table.size == 0u) {
        unsigned int new_size = INITIAL_TABLE_SIZE;
        stack_entry_t* arr = (stack_entry_t*) calloc(new_size, sizeof(stack_entry_t));
        if (!arr) return 0;
        g_table.entries = arr;
        g_table.size = new_size;
    }
    return 1;
}

// увеличить таблицу до  new_min_size слотов. 1 при успехе, 0 при ошибке
static int expand_table(unsigned int new_min_size)
{
    if (new_min_size <= g_table.size) return 1;
    unsigned int cur = (g_table.size == 0 ? INITIAL_TABLE_SIZE : g_table.size);
    while (cur < new_min_size) {
        if (cur > UINT_MAX / 2) {
            cur = new_min_size;
            break;
        }
        cur *= 2;
    }
    stack_entry_t* new_arr = (stack_entry_t*) realloc(g_table.entries, (size_t)cur * sizeof(stack_entry_t));
    if (!new_arr) return 0;
    //инициализируем новые слоты нулями
    if (cur > g_table.size) {
        size_t diff = (size_t)(cur - g_table.size);
        memset(new_arr + g_table.size, 0, diff * sizeof(stack_entry_t));
    }
    g_table.entries = new_arr;
    g_table.size = cur;
    return 1;
}

// найти первый свободный слот, при необходимости расширяя таблицу. возвращает индекс >=0 при успехе, -1 при ошибке
static int find_and_reserve_slot(void)
{
    if (!ensure_table_initialized()) return -1;
    //сначала поиск свободного слота
    for (unsigned int i = 0; i < g_table.size; ++i) {
        if (!g_table.entries[i].used) {
            g_table.entries[i].used = 1;
            g_table.entries[i].top = NULL;
            g_table.entries[i].count = 0;
            return (int)i;
        }
    }
    unsigned int old_size = g_table.size;
    if (!expand_table(g_table.size + 1)) return -1;
    for (unsigned int i = old_size; i < g_table.size; ++i) {
        if (!g_table.entries[i].used) {
            g_table.entries[i].used = 1;
            g_table.entries[i].top = NULL;
            g_table.entries[i].count = 0;
            return (int)i;
        }
    }
    //на всякий случай
    return -1;
}

//безопасная проверка индекса-обработчика
static int handler_in_range(const hstack_t h)
{
    if (h < 0) return 0;
    unsigned int idx = (unsigned int)h;
    if (g_table.size == 0) return 0;
    if (idx >= g_table.size) return 0;
    return 1;
}

hstack_t stack_new(void)
{
    int idx = find_and_reserve_slot();
    if (idx < 0) return -1;
    return (hstack_t)idx;
}

void stack_free(const hstack_t hstack)
{
    //дескриптор некорректен или не существует — ничего не делаем
    if (!handler_in_range(hstack)) return;
    unsigned int idx = (unsigned int)hstack;
    stack_entry_t* ent = &g_table.entries[idx];
    if (!ent->used) return;
    //освобождаем все узлы стека
    node_t* cur = ent->top;
    while (cur) {
        node_t* prev = cur->prev;
        free(cur);
        cur = prev;
    }
    //очищаем запись 
    ent->top = NULL;
    ent->count = 0;
    ent->used = 0;
}

int stack_valid_handler(const hstack_t hstack)
{
    //0 — стек существует, 1 — нет 
    if (!handler_in_range(hstack)) return 1;
    unsigned int idx = (unsigned int)hstack;
    if (g_table.entries[idx].used) return 0;
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    if (!handler_in_range(hstack)) return 0;
    unsigned int idx = (unsigned int)hstack;
    stack_entry_t* ent = &g_table.entries[idx];
    if (!ent->used) return 0;
    return ent->count;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    //проверки входных параметров
    if (!handler_in_range(hstack)) return;
    if (data_in == NULL) return;
    if (size == 0u) return;

    unsigned int idx = (unsigned int)hstack;
    stack_entry_t* ent = &g_table.entries[idx];
    if (!ent->used) return;

    //защита от больших размеров

    node_t* n = (node_t*) malloc(sizeof(node_t) + size);
    if (!n) return;

    n->prev = ent->top;
    n->size = size;
    memcpy(n->data, data_in, size);

    ent->top = n;
    //защита от переполнения count
    if (ent->count < UINT_MAX) ent->count += 1;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (!handler_in_range(hstack)) return 0;
    if (data_out == NULL) return 0;
    if (size == 0u) return 0;

    unsigned int idx = (unsigned int)hstack;
    stack_entry_t* ent = &g_table.entries[idx];
    if (!ent->used) return 0;
    if (ent->top == NULL) return 0; //пустой стек

    node_t* n = ent->top;
    if (size < n->size)
	    return 0;
    unsigned int to_copy = (n->size <= size) ? n->size : size;
    memcpy(data_out, n->data, to_copy);

    ent->top = n->prev;
    if (ent->count > 0) ent->count -= 1;
    free(n);

    return to_copy;
}

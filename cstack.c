#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h> // для работы с памятью

#define UNUSED(VAR) (void)(VAR)

struct node {
    struct node* prev;
    unsigned int size;
    char data[];
};

typedef struct node* stack_t;

struct stack_entry {
    int used;
    stack_t top;
    unsigned int count;
};

static struct {
    unsigned int capacity;
    struct stack_entry* entries;
} g_table = { 0u, NULL };

static int ensure_capacity(unsigned int min_capacity) {
    if (g_table.capacity >= min_capacity)
        return 1;

    unsigned int new_capacity = g_table.capacity ? g_table.capacity * 2u : 16u;
    if (new_capacity < min_capacity)
        new_capacity = min_capacity;

    struct stack_entry* new_entries = (struct stack_entry*)realloc(g_table.entries, new_capacity * sizeof(struct stack_entry));
    if (!new_entries)
        return 0;

    // Инициализация новых слотов
    for (unsigned int i = g_table.capacity; i < new_capacity; ++i) {
        new_entries[i].used = 0;
        new_entries[i].top = NULL;
        new_entries[i].count = 0u;
    }

    g_table.entries = new_entries;
    g_table.capacity = new_capacity;
    return 1;
}

hstack_t stack_new(void) {
    // Нужно найти свободный слот или расширить таблицу
    if (!ensure_capacity(1u))
        return -1;

    for (unsigned int i = 0; i < g_table.capacity; ++i) {
        if (!g_table.entries[i].used) {
            g_table.entries[i].used = 1;
            g_table.entries[i].top = NULL;
            g_table.entries[i].count = 0u;
            return (hstack_t)i;
        }
    }

    // Свободных слотов не нашлось, расширяем и используем дальше
    if (!ensure_capacity(g_table.capacity + 1u))
        return -1;

    unsigned int idx = g_table.capacity - 1u;
    g_table.entries[idx].used = 1;
    g_table.entries[idx].top = NULL;
    g_table.entries[idx].count = 0u;
    return (hstack_t)idx;
}

void stack_free(const hstack_t hstack) {
    if (hstack < 0)
        return;
    unsigned int idx = (unsigned int)hstack;
    if (idx >= g_table.capacity)
        return;
    if (!g_table.entries[idx].used)
        return;

    // Освобождение всех нодов
    stack_t cur = g_table.entries[idx].top;
    while (cur) {
        stack_t prev = cur->prev;
        free(cur);
        cur = prev;
    }

    g_table.entries[idx].used = 0;
    g_table.entries[idx].top = NULL;
    g_table.entries[idx].count = 0u;
}

int stack_valid_handler(const hstack_t hstack) {
    // Проверка хэндлера стека
    if (hstack < 0)
        return 1;
    unsigned int idx = (unsigned int)hstack;
    if (idx >= g_table.capacity)
        return 1;
    return g_table.entries[idx].used ? 0 : 1;
}

unsigned int stack_size(const hstack_t hstack) {
    // Узнать размер стека с проверками
    if (hstack < 0)
        return 0u;
    unsigned int idx = (unsigned int)hstack;
    if (idx >= g_table.capacity)
        return 0u;
    if (!g_table.entries[idx].used)
        return 0u;
    return g_table.entries[idx].count;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size) {
    // Проверка входных данных
    if (hstack < 0)
        return;
    if (!data_in)
        return;
    if (size == 0u)
        return;

    unsigned int idx = (unsigned int)hstack;
    if (idx >= g_table.capacity)
        return;
    if (!g_table.entries[idx].used)
        return;

    // Выделить нод с расширяющимся массивом для данных
    size_t alloc_size = sizeof(struct node) + (size_t)size;
    struct node* n = (struct node*)malloc(alloc_size);
    if (!n)
        return;

    n->prev = g_table.entries[idx].top;
    n->size = size;
    memcpy(n->data, data_in, size);

    g_table.entries[idx].top = n;
    g_table.entries[idx].count += 1u;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size) {
    if (hstack < 0)
        return 0u;
    unsigned int idx = (unsigned int)hstack;
    if (idx >= g_table.capacity)
        return 0u;
    if (!g_table.entries[idx].used)
        return 0u;

    stack_t top = g_table.entries[idx].top;
    if (!top)
        return 0u;

    // Проверить указатель на выходной буфер
    if (!data_out)
        return 0u;
    if (size < top->size)
        return 0u;

    unsigned int copied = top->size;
    memcpy(data_out, top->data, copied);

    g_table.entries[idx].top = top->prev;
    g_table.entries[idx].count -= 1u;
    free(top);

    return copied;
}
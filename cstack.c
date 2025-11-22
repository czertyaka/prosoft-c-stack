#include "cstack.h"
#include <stdlib.h>
#include <string.h>

#define MAX_STACKS 10

#pragma warning(push)
#pragma warning(disable:5045)

#pragma warning(push)
#pragma warning(disable:4820) // отключаем C4820

struct node {
    struct node* prev;
    unsigned int size;
    char* data; // данные отдельно
};

#pragma warning(pop)

typedef struct node* stack_t;

struct stack_entry {
    int used;
    unsigned int count;
    stack_t top;
};

static struct stack_entry table[MAX_STACKS] = { 0 };

hstack_t stack_new(void) {
    for (int i = 0; i < MAX_STACKS; ++i) {
        if (!table[i].used) {
            table[i].used = 1;
            table[i].count = 0;
            table[i].top = NULL;
            return i;
        }
    }
    return -1;
}

void stack_free(const hstack_t stack) {
    if (stack < 0 || stack >= MAX_STACKS || !table[stack].used)
        return;

    stack_t cur = table[stack].top;
    while (cur) {
        stack_t prev = cur->prev;
        free(cur->data);
        free(cur);
        cur = prev;
    }

    table[stack].used = 0;
    table[stack].count = 0;
    table[stack].top = NULL;
}

int stack_valid_handler(const hstack_t stack) {
    if (stack < 0 || stack >= MAX_STACKS || !table[stack].used)
        return 1;
    return 0;
}

unsigned int stack_size(const hstack_t stack) {
    if (stack < 0 || stack >= MAX_STACKS || !table[stack].used)
        return 0;
    return table[stack].count;
}

void stack_push(const hstack_t stack, const void* data_in, const unsigned int size) {
    if (stack < 0 || stack >= MAX_STACKS || !table[stack].used)
        return;

    if (size > 0 && data_in == NULL)
        return;

    stack_t n = (stack_t)malloc(sizeof(struct node));
    if (!n) return;

    n->prev = table[stack].top;
    n->size = size;

    if (size > 0) {
        n->data = malloc(size);
        if (!n->data) { free(n); return; }
        memcpy(n->data, data_in, size);
    }
    else {
        n->data = NULL;
    }

    table[stack].top = n;
    ++table[stack].count;
}

unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int buf_size) {
    if (stack < 0 || stack >= MAX_STACKS || !table[stack].used)
        return 0;

    stack_t top = table[stack].top;
    if (!top) return 0;

    unsigned int written = 0;
    if (data_out != NULL && buf_size > 0 && top->size > 0) {
        written = (top->size <= buf_size ? top->size : buf_size);
        memcpy(data_out, top->data, written);
    }

    table[stack].top = top->prev;
    free(top->data);
    free(top);
    --table[stack].count;

    return written;
}

#pragma warning(pop)

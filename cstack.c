#include "cstack.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STACKS 1024

typedef struct stack_node {
    struct stack_node* next;
    unsigned int size;
    uint8_t data[];
} stack_node_t;

typedef struct {
    stack_node_t* top;
    unsigned int count;
    int in_use;
} stack_t;

static stack_t stacks[MAX_STACKS] = {0};

hstack_t stack_new(void)
{
    for (int i = 0; i < MAX_STACKS; i++) {
        if (!stacks[i].in_use) {
            stacks[i].in_use = 1;
            stacks[i].top = NULL;
            stacks[i].count = 0;
            return i;
        }
    }
    return -1;  // no free slots
}

void stack_free(const hstack_t hstack)
{
    if (hstack < 0 || hstack >= MAX_STACKS || !stacks[hstack].in_use)
        return;

    stack_node_t* node = stacks[hstack].top;
    while (node) {
        stack_node_t* next = node->next;
        free(node);
        node = next;
    }

    stacks[hstack].top = NULL;
    stacks[hstack].count = 0;
    stacks[hstack].in_use = 0;
}

int stack_valid_handler(const hstack_t hstack)
{
    return (hstack >= 0 && hstack < MAX_STACKS && stacks[hstack].in_use);
}

unsigned int stack_size(const hstack_t hstack)
{
    if (!stack_valid_handler(hstack))
        return 0;
    return stacks[hstack].count;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (!stack_valid_handler(hstack) || !data_in || size == 0)
        return;

    stack_node_t* node = malloc(sizeof(stack_node_t) + size);
    if (!node) return;

    node->size = size;
    memcpy(node->data, data_in, size);

    node->next = stacks[hstack].top;
    stacks[hstack].top = node;
    stacks[hstack].count++;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (!stack_valid_handler(hstack))
        return 0;

    stack_node_t* node = stacks[hstack].top;
    if (!node)
        return 0;

    unsigned int to_copy = (size < node->size ? size : node->size);

    if (data_out && to_copy > 0)
        memcpy(data_out, node->data, to_copy);

    stacks[hstack].top = node->next;
    stacks[hstack].count--;
    free(node);

    return to_copy;
}

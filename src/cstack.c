#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int bool_in_c;
enum
{
    false,
    true
};

stack_head stacks_head[_MAX_STACKS] = {{((void *)0), 0, true}};

hstack_t stack_new()
{
    for (int id = 0; id < _MAX_STACKS; id++)
        if (stacks_head[id].is_free)
        {
            stacks_head[id].is_free = false;
            return id;
        }

    printf("There are no free staks! \nMaximum number of stacks: %d.\n", _MAX_STACKS);
    return -1;
}

void stack_free(const hstack_t stack_id)
{
    if (stack_id > _MAX_STACKS || stack_id < 0)
        return;

    if (stacks_head[stack_id].is_free)
        return;

    node *iterator = stacks_head[stack_id].head_pointer;
    for (unsigned int i = 0; i < stacks_head[stack_id].size; i++)
    {
        free(iterator->data);
        node *temp = iterator;
        node *iterator = iterator->prev;
        free(temp);
    }

    stacks_head[stack_id].head_pointer = NULL;
    stacks_head[stack_id].is_free = true;
    stacks_head[stack_id].size = 0;
    return;
}

int stack_valid_handler(const hstack_t stack_id)
{
    if (stack_id > _MAX_STACKS || stack_id < 0)
        return 1;

    return stacks_head[stack_id].is_free;
}

unsigned int stack_size(const hstack_t stack_id)
{
    if (stack_id > _MAX_STACKS || stack_id < 0)
        return 0u;

    if (stacks_head[stack_id].is_free)
        return 0u;

    return stacks_head[stack_id].size;
}

int stack_push(const hstack_t stack_id, const void *data_in, const unsigned int size)
{
    if (stack_id > _MAX_STACKS || stack_id < 0)
        return 0;
    if (stacks_head[stack_id].is_free)
        return 0;
    if (data_in == NULL)
        return 0;


    node *new_node = malloc(sizeof(node));
    if (new_node == NULL)
        return 0;

    new_node->data = malloc(size);
    if (new_node->data == NULL)
        return 0;
    new_node->size = size;

    memcpy(new_node->data, data_in, size);

    new_node->prev = stacks_head[stack_id].head_pointer;
    stacks_head[stack_id].head_pointer = new_node;
    stacks_head[stack_id].size += 1;
    return 1;
}

unsigned int stack_pop(const hstack_t stack_id, void *data_out, const unsigned int size)
{
    if (stack_id > _MAX_STACKS || stack_id < 0)
        return 0;
    if (stacks_head[stack_id].is_free)
        return 0;
    if (data_out == NULL)
        return 0;
    if (size < stacks_head[stack_id].head_pointer->size)
        return 0;
    if (stacks_head[stack_id].size == 0)
        return 0;

    memcpy(data_out, stacks_head[stack_id].head_pointer->data, size);

    free(stacks_head[stack_id].head_pointer->data);
    node *temp = stacks_head[stack_id].head_pointer;
    stacks_head[stack_id].head_pointer = stacks_head[stack_id].head_pointer->prev;
    stacks_head[stack_id].size -= 1;
    free(temp);

    return 1;
}

#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

hstack_id stack_new()
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

void stack_free(const hstack_id stack_id)
{
    UNUSED(stack_id);
    node* iterator = stacks_head[stack_id].head_pointer;
    for (int i = 0; i < stacks_head[stack_id].size; i++)
    {
        free(iterator->data);
        node* temp = iterator;
        iterator = iterator->prev;
        free(temp);
    }
    return;
}

int stack_valid_handler(const hstack_id stack_id)
{
    return stacks_head[stack_id].is_free;
}

unsigned int stack_size(const hstack_id stack_id)
{
    return stacks_head[stack_id].size;
}

int stack_push(const hstack_id stack_id, const void *data_in, const unsigned int size)
{
    if (data_in == NULL)
        return 0;

    node *new_node = malloc(sizeof(node));
    if (new_node == NULL)
        return 0;

    new_node->data = malloc(size);
    if (new_node->data == NULL)
        return 0;
    new_node->size = size;

    memcpy(new_node->data,data_in,size);

    new_node->prev = stacks_head[stack_id].head_pointer;
    stacks_head[stack_id].head_pointer = new_node;
    stacks_head[stack_id].size+= 1;
    return 1;
}

unsigned int stack_pop(const hstack_id stack_id, void *data_out, const unsigned int size)
{
    if (data_out == NULL)
        return 0;
     if (size < stacks_head[stack_id].head_pointer->size)
        return 0;
    if (stacks_head[stack_id].size == 0)
        return 0;

    memcpy(data_out,stacks_head[stack_id].head_pointer->data,size);

    free(stacks_head[stack_id].head_pointer->data);
    node* temp = stacks_head[stack_id].head_pointer;
    stacks_head[stack_id].head_pointer = stacks_head[stack_id].head_pointer->prev;
    stacks_head[stack_id].size -= 1;
    free(temp);

    return 1;
}

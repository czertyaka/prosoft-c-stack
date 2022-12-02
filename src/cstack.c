#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#define UNUSED(VAR) (void)(VAR)


hstack_t stack_new()
{
    if (entries == 0)
    {
  	  staks = stack_new();    
      entries = 1;
    }

    node* zero_node = malloc(sizeof(node));
    zero_node->data = NULL;
    zero_node->prev = NULL;
    zero_node->size = 0;

    stack_push(staks,zero_node,sizeof(zero_node));
    entries = entries+1;

    
    return -1;
}


void stack_free(const hstack_t hstack)
{
    UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{
    UNUSED(hstack);
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    UNUSED(hstack);
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_in);
    UNUSED(size);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_out);
    UNUSED(size);
    return 0;
}


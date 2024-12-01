#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

struct node{
	struct node* prev;
	unsigned int size;
	char data[];
};

typedef struct node node_t;

struct stack{
	node_t* end;
	unsigned int size;
};

typedef struct stack stack_t;

struct stack_table{
	stack_t** stacks;
	unsigned int size;
};

typedef struct stack_table stack_table_t;


stack_table_t stable = {NULL, 0u};


hstack_t stack_new(void)
{
	stack_t** new_stacks = malloc(sizeof(stack_t*) * (stable.size + 1));
	memcpy(new_stacks, stable.stacks, sizeof(stack_t*) * stable.size);
	if (stable.stacks != NULL)
		free(stable.stacks);

	stable.stacks = new_stacks;
	stable.size += 1;

	stack_t* new_stack = malloc(sizeof(stack_t));
	new_stack->end = NULL;
	new_stack->size = 0;
	stable.stacks[stable.size - 1] = new_stack;

    return stable.size - 1;
}

void stack_free(const hstack_t hstack)
{
	if (stack_valid_handler(hstack) == 1)
		return;

	stack_t* stack = stable.stacks[hstack];

	node_t* end = stack->end;
	while (end != NULL){
		node_t* prev = end->prev;
		free(end);
		end = prev;
	}	

	free(stack);
	stable.stacks[hstack] = NULL;
}

int stack_valid_handler(const hstack_t hstack)
{
	if ((unsigned int)hstack >= stable.size)
		return 1;
	stack_t* stack = stable.stacks[hstack];
	return (stack == NULL);
}

unsigned int stack_size(const hstack_t hstack)
{
	if (stack_valid_handler(hstack) == 1)
		return 0;
	return stable.stacks[hstack]->size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
	if (stack_valid_handler(hstack) == 1 || data_in == NULL || size == 0)
		return;

	stack_t* stack = stable.stacks[hstack];

	node_t* new_node = malloc(sizeof(node_t) + size);
	new_node->prev = stack->end;
	stack->end = new_node;
	stack->size += 1;

	new_node->size = size;
	memcpy(new_node->data, data_in, size);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
	if (stack_valid_handler(hstack) == 1 || data_out == NULL)
		return 0;

	stack_t* stack = stable.stacks[hstack];
	if (stack->size == 0)
		return 0;
	
	node_t* end = stack->end;
	size_t node_size = end->size;
	if (node_size > size)
		return 0;

	memcpy(data_out, end->data, node_size);

	stack->end = end->prev;
	stack->size -= 1;
	free(end);
	
	return(node_size);
}


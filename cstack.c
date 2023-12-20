#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define UNUSED(VAR) (void)(VAR)


#define MAX_COUNT_STACK 10
#define hstack_t int
struct node
{
	const struct node* prev;
	unsigned int size;
	char* data;
};

typedef struct node* stack_t;

struct stack_entry
{
	int reserved;
	stack_t stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
	unsigned int size;
	stack_entry_t* entries[MAX_COUNT_STACK];
};

struct stack_entries_table g_table = {0u, {NULL} };

hstack_t stack_new(void)
{
	if (g_table.size == MAX_COUNT_STACK)
	{
		return -1;
	}

	stack_entry_t* new_stack = (stack_entry_t*)malloc(sizeof(stack_entry_t));
	new_stack -> reserved = 0;
	new_stack -> stack = NULL;

	if (!new_stack)
	{
		return -1;
	}

	for (size_t i = 0; i < MAX_COUNT_STACK; ++i)
	{
		if (!g_table.entries[i])
		{
			g_table.entries[i] = new_stack;
			++g_table.size;
			return i;
		}
	}

	return -1;
}

void stack_free(const hstack_t hstack)
{
	if (stack_valid_handler(hstack))
	{
		return;
	}

	stack_t curr_stack = g_table.entries[hstack]->stack;

	if (curr_stack)
	{
		stack_t prev_stack = (stack_t)curr_stack->prev;
		for (int i = 0; i < g_table.entries[hstack]->reserved; ++i)
		{
			free(curr_stack->data);
			curr_stack = prev_stack;
			if (curr_stack)
			{
				prev_stack = (stack_t) curr_stack->prev;
			}
		}
		free(g_table.entries[hstack]->stack);
		free(g_table.entries[hstack]);
	}

	g_table.entries[hstack] = NULL;
	--g_table.size;
}

int stack_valid_handler(const hstack_t hstack)
{
	if (hstack < MAX_COUNT_STACK && hstack >= 0 && g_table.entries[hstack] != NULL)
	{
		return 0;
	}

	return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
	if (!stack_valid_handler(hstack))
	{
		return g_table.entries[hstack]->reserved;
	}

	return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
	if (!stack_valid_handler(hstack) && data_in && size > 0)
	{
		stack_t new_node = (stack_t) malloc(sizeof(struct node));
		if (!new_node)
		{
			return;
		}

		new_node->data = (char*) malloc(sizeof (char) * size);

		new_node->size = size;
		new_node->prev = g_table.entries[hstack]->stack;
		memcpy(new_node->data, data_in, size);
		g_table.entries[hstack]->stack = new_node;
		++g_table.entries[hstack]->reserved;
	}


}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
	if (!stack_valid_handler(hstack)
			&& g_table.entries[hstack]->reserved > 0
			&& size >= g_table.entries[hstack]->stack->size
			&& data_out)
	{
		stack_t pop_element = g_table.entries[hstack]->stack;
		stack_t new_top_element = (stack_t)pop_element->prev;

		memcpy(data_out,
			   pop_element->data,
			   pop_element->size);

		g_table.entries[hstack]->stack = new_top_element;
		unsigned int copy_size = pop_element->size;
		free(pop_element->data);
		free(pop_element);

		--g_table.entries[hstack]->reserved;

		return copy_size;
	}
	return 0;
}




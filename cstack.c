#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct element
{
	struct element* previous;
	unsigned int size;
	char data[0];
} element;

typedef struct stack
{
	element* top_element;
	unsigned int size;
} stack;

typedef struct stack_entry
{
	bool is_valid;
	stack stack;
} stack_entry;

typedef struct stack_table
{
	hstack_t size;
	stack_entry* stack_entries;
} stack_table;

stack_table stack_t = { 0u, NULL };

bool allocate_table(void)
{
	stack_t.stack_entries = calloc(1, sizeof(stack_entry));
	if (stack_t.stack_entries)
	{
		stack_t.size = 1;
		return true;
	}
	else
		return false;
}

void reallocate_table(void)
{
	hstack_t new_size = stack_t.size + 1;
	stack_entry* new_array = calloc(new_size, sizeof(stack_entry));

	if (new_array)
	{
		memcpy(new_array, stack_t.stack_entries, stack_t.size * sizeof(stack_entry));
		free(stack_t.stack_entries);
		stack_t.stack_entries = new_array;
		stack_t.size = new_size;
	}
}

hstack_t get_free_index(void)
{
	hstack_t index = 0;
	for (; index < stack_t.size && stack_t.stack_entries[index].is_valid; ++index);
	return index;
}

hstack_t stack_new(void)
{
	if (!stack_t.stack_entries && !allocate_table())
		return -1;

	hstack_t index = get_free_index();

	if (index >= stack_t.size)
		reallocate_table();

	if (index < stack_t.size)
	{
		stack_t.stack_entries[index].is_valid = true;
		return index;
	}
	return -1;
}

void stack_free(const hstack_t hstack)
{
	if (stack_valid_handler(hstack) != 0)
		return;

	stack* stack = &stack_t.stack_entries[hstack].stack;
	element* cur_element = stack->top_element;

	stack_t.stack_entries[hstack].is_valid = false;
	stack->top_element = NULL;
	stack->size = 0;

	element* prev_element;
	while (cur_element)
	{
		prev_element = cur_element->previous;
		free(cur_element);
		cur_element = prev_element;
	}

	free(stack_t.stack_entries);
	stack_t.stack_entries = NULL;
	stack_t.size = 0;
}

int stack_valid_handler(const hstack_t hstack)
{
	return hstack >= 0 && hstack < stack_t.size && stack_t.stack_entries[hstack].is_valid ? 0 : 1;
}

unsigned int stack_size(const hstack_t hstack)
{
	return stack_valid_handler(hstack) == 0 ? stack_t.stack_entries[hstack].stack.size : 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
	if (!data_in || !size || stack_valid_handler(hstack) != 0)
		return;

	stack* stack = &stack_t.stack_entries[hstack].stack;
	if (stack->size == UINT_MAX)
		return;

	element* new_element = calloc(1, sizeof(element) + size);
	if (!new_element)
		return;

	new_element->previous = stack->top_element;
	new_element->size = size;
	memcpy(new_element->data, data_in, size);

	stack->top_element = new_element;
	stack->size += 1;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
	if (!data_out || !size || stack_valid_handler(hstack) != 0)
		return 0;

	stack* stack = &stack_t.stack_entries[hstack].stack;
	element* cur_element = stack->top_element;

	if (!cur_element)
		return 0;

	unsigned int data_size = cur_element->size;
	if (data_size > size)
		return 0;

	stack->top_element = cur_element->previous;
	stack->size -= 1;

	memcpy(data_out, cur_element->data, data_size);
	free(cur_element);
	return data_size;
}

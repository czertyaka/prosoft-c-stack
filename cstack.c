#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 11

#pragma pack(push,1)
typedef struct  node
{
	struct node* prev;
	unsigned int size;
	void* data;
} stack_t;

typedef struct stack_entry
{
	unsigned int reserved;
	stack_t* entry;
} stack_entry_t;

typedef struct stack_entries_table
{
	unsigned int size;
	stack_entry_t* entries[INIT_SIZE];
} stack_entries_table;
#pragma pack(pop)

stack_entries_table g_table = { 0u, NULL };

hstack_t stack_new(void) {
	if (g_table.size == INIT_SIZE - 1)
		return -1;

	stack_entry_t* stack = (stack_entry_t*)malloc(sizeof(stack_entry_t));
	if (!stack)
		return -1;

	stack->reserved = 0;
	stack->entry = NULL;
	g_table.entries[g_table.size] = stack;
	return g_table.size++;
}

void stack_free(const hstack_t hstack) {
	if (stack_valid_handler(hstack))
		return;

	stack_entry_t* stack = g_table.entries[hstack];
	if (stack->entry)
	{
		stack_t* prev = NULL;
		stack_t* current = stack->entry;
		while (current) {
			prev = current->prev;
			free(current);
			current = prev;
		}
	}
	free(stack);
	g_table.entries[hstack] = NULL;
	--g_table.size;
}

int stack_valid_handler(const hstack_t hstack) {
	return hstack < 0 || hstack >= INIT_SIZE ||
		//!g_table.entries[hstack]->reserved || 
		g_table.entries[hstack] == NULL;
}

unsigned int stack_size(const hstack_t hstack) {
	return (stack_valid_handler(hstack) == 0) ? g_table.entries[hstack]->reserved : 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size) {
	if (stack_valid_handler(hstack) || !data_in || !size)
		return;

	stack_t* head = (stack_t*)malloc(sizeof(stack_t));
	if (!head)
		return;

	head->size = size;
	head->data = malloc(size);
	if (!head->data) {
		free(head);
		return;
	}

	memcpy_s(head->data, head->size, data_in, size);
	head->prev = g_table.entries[hstack]->entry;

	g_table.entries[hstack]->entry = head;
	g_table.entries[hstack]->reserved += 1;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size) {
	if (stack_valid_handler(hstack) ||
		!data_out || !size ||
		!g_table.entries[hstack]->entry ||
		g_table.entries[hstack]->entry->size > size) {
		return 0;
	}

	memcpy_s(data_out, size, g_table.entries[hstack]->entry->data, g_table.entries[hstack]->entry->size);
	stack_t* head = g_table.entries[hstack]->entry->prev;

	free(g_table.entries[hstack]->entry->data);
	free(g_table.entries[hstack]->entry);

	g_table.entries[hstack]->entry = head;
	g_table.entries[hstack]->reserved -= 1;
	return size;
}

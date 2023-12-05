#include "cstack.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
	void* data[MAX_SIZE];
	unsigned int size;
} stack_t;

typedef stack_t* hstack_t;

hstack_t stack_new(void) {
	hstack_t stack = malloc(sizeof(stack_t));
	if (stack == NULL) {
		return NULL;
	}
	stack->size = 0;
	return stack;
}

void stack_free(const hstack_t stack) {
	if (stack != NULL) {
		free(stack);
	}
}

int stack_valid_handler(const hstack_t stack) {
	if (stack == NULL) {
		return 1;
	}
	return 0;
}

unsigned int stack_size(const hstack_t stack) {
	if (stack != NULL) {
		return stack->size;
	}
	return 0;
}

void stack_push(const hstack_t stack, const void* data_in, const unsigned int size) {
	if (stack != NULL && stack->size < MAX_SIZE) {
		stack->data[stack->size] = malloc(size);
		if (stack->data[stack->size] != NULL) {
			memcpy(stack->data[stack->size], data_in, size);
			stack->size++;
		}
	}
}

unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size) {
	unsigned int dataSize = 0;
	if (stack != NULL && stack->size > 0 && data_out != NULL) {
		dataSize = size <= stack->size ? size : stack->size;
		memcpy(data_out, stack->data[stack->size - 1], dataSize);
		free(stack->data[stack->size - 1]);
		stack->size--;
	}
	return dataSize;
}
#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define UNUSED(VAR) (void)(VAR)

struct node
{
	struct node *prev_node;
	void *data_pointer;
	unsigned int data_size;
};

struct stack
{
	struct node *top_of_stack;
	unsigned int number_of_stack_elements;
	char stack_exists;
};

struct stack_t{
	unsigned int table_volume;
	unsigned int table_size;
	struct stack *stack_list;
};

struct stack_t *stack_table = NULL;
struct node *node;
void *copy_data_pointer;

hstack_t stack_new()
{
	if(!stack_table)
	{
		stack_table = (struct stack_t*) malloc(sizeof(struct stack_t));
		stack_table->table_volume = 10;
		stack_table->table_size = 1;
		stack_table->stack_list = (struct stack*) malloc(sizeof(struct stack) * stack_table->table_volume);
	}
	stack_table->stack_list[stack_table->table_size].top_of_stack = NULL;
	stack_table->stack_list[stack_table->table_size].number_of_stack_elements = 0;
	stack_table->stack_list[stack_table->table_size].stack_exists = 0;
	stack_table->table_size++;
	if (stack_table->table_size == stack_table->table_volume)
	{
		stack_table->table_volume *= 2;
		stack_table->stack_list = (struct stack*) realloc(stack_table->stack_list, sizeof(struct stack) * stack_table->table_volume);
	}
	if (stack_table->stack_list == NULL)
	{
		return -1;
	}
    return stack_table->table_size - 1;
}

void stack_free(const hstack_t hstack)
{
	while(stack_table->stack_list[hstack].top_of_stack != NULL)
	{
		struct node *node_pointer = stack_table->stack_list[hstack].top_of_stack; // Копирование указателя на node
		stack_table->stack_list[hstack].top_of_stack = node->prev_node; // Изменение значения верхушки стека
		free(node_pointer); // Освобождаем память удаляемого элемета
	}
    stack_table->stack_list[hstack].stack_exists = 1; // Флаг "несуществования" стека
}

int stack_valid_handler(const hstack_t hstack)
{
    return stack_table->stack_list[hstack].stack_exists;
}

unsigned int stack_size(const hstack_t hstack)
{
    return stack_table->stack_list[hstack].number_of_stack_elements;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
	if(data_in == NULL)
	{
		printf("Error: null pointer");
	}
	if(size == 0)
	{
		printf("Error: null size");
	}
	node = (struct node*) malloc(sizeof(struct node));  // Выделили память под node и получили указатель
	node->prev_node = stack_table->stack_list[hstack].top_of_stack; // В указатель на предыдущий node поместили указатель на верхушку стека
	stack_table->stack_list[hstack].top_of_stack = node; // В указатель верхушки стека поместили указатель на последний node
	copy_data_pointer = (void*) malloc(size); // Выделяем память для копирования данных
	memcpy(copy_data_pointer, data_in, size); // Копирование данных
	node->data_pointer = copy_data_pointer; // Передаем в node указатель на данные
	node->data_size = size; // Передаем в node размер данных
	stack_table->stack_list[hstack].number_of_stack_elements++; // Увеличиваем количество элементов в стеке
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
	if(data_out == NULL || size < node->data_size) // Проверка на нулевой указатель и на размер буфера
	{
		return 0;
	}
	memcpy(data_out, node->data_pointer, node->data_size); // Перенос данных в буфер
	unsigned int out_data_size = node->data_size; // Значение размера выходных данных
	struct node *node_pointer = stack_table->stack_list[hstack].top_of_stack; // Копирование указателя на node
	stack_table->stack_list[hstack].top_of_stack = node->prev_node; // Изменение значения верхушки стека
	stack_table->stack_list[hstack].number_of_stack_elements--; // Изменение значения количества элементов в стеке
	free(node_pointer); // Освобождаем память извлекаемого элемета
	free(copy_data_pointer); // Освобождаем память куда копировали данные
	return out_data_size;
}
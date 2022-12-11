#include "cstack.h"
#include "cstack.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef int hstack_t;
typedef struct Node Node;

#define UNUSED(VAR) (void)(VAR)
struct Node
{
	void* field; // поле данных
	Node *next; // указатель на следующий элемент
};
typedef struct stack_array stack_array;
struct stack_array
{
	unsigned int size;
	Node* entries;

};

stack_array *arr;
// Инициализация корня узла
stack_array* array_initial()
{
	arr = (stack_array*)malloc(sizeof(stack_array));
	arr->size = 0;
	arr->entries = NULL;

	if (arr)
	{
		return arr;
	}
}
hstack_t stack_new()
{
	//выделение памяти под корень списка
	Node *root = (Node*)malloc(sizeof(Node));
	if (!root)
	{
		return -1;
	}
	root->field = NULL;
	root->next = NULL;
	if (arr == NULL)
	{
		array_initial();
		if (!arr)
		{
			return -1;
		}
	}
	static unsigned int numb = 0;
	arr->size = numb;
	stack_array *arr_next = arr + arr->size;
	arr_next->entries = root;
	arr_next->size = 1;
	return numb++;
}

void stack_free(const hstack_t hstack)
{
	if (hstack < 0 || hstack > arr->size)
	{
		printf("wrong data");
	}
	else
	{
		stack_array *entries = arr + hstack;
		Node *top = entries->entries;
		if (top->field != NULL)
		{
			while (top->field != NULL)
			{
				Node * pre_top = top->next;
				entries->entries = pre_top;
				free(top->field);
				free(top);
				top = pre_top;
			}
			free(top);
			entries->size = 0;
		}
		else
		{
			free(top);
			entries->size = 0;
		}
	}
}

int stack_valid_handler(const hstack_t hstack)
{
	if (hstack < 0 || hstack > arr->size)
	{
		return 1;
	}
	else
	{
		stack_array *entries = arr + hstack;
		Node *top = entries->entries;
		if (entries->size != 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

unsigned int stack_size(const hstack_t hstack)
{
	if (hstack <0 || hstack > arr->size)
	{
		return 0u;
	}
	else
	{
		stack_array *entries = arr + hstack;
		Node *top = entries->entries;
		if (top->field == NULL)
		{
			return 0;
		}
		else
		{
			unsigned int number = 0;
			while (top->field != NULL)
			{
				number++;
				top = top->next;
			}
			return number;
		}
	}
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
	if (hstack < 0 || data_in == NULL || size == NULL || hstack > arr->size)
	{
		printf ("wrong data");
	}
	else
	{
		stack_array *entries = arr + hstack;
		Node *top = entries->entries;
		Node *new_node = (Node *)malloc(sizeof(Node));
		new_node->field = malloc(size);
		if (new_node)
		{
			memcpy(new_node->field, data_in, size);
			new_node->next = top;
			entries->entries = new_node;
		}
	}
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
	if (hstack < 0 || size == NULL || hstack > arr->size || data_out == NULL)
	{
		return 0u;
	}
	else
	{
		stack_array *entries = arr + hstack;
		Node *top = entries->entries;
		if (top->field != NULL)
		{
			if (size == sizeof(top->field))
			{
				data_out = top->field;
				Node *pre_top = top->next;
				entries->entries = pre_top;
				free(top->field);
				free(top);
				return sizeof(data_out);
			}
			else
			{
				return 0u;
			}
		}
		else
		{
			return 0u;
		}
	}
}

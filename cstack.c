#include "cstack.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define UNUSED(VAR) (void)(VAR)
// Объявление списка стеков
struct stack_table
{
    size_t size;
    stack_type* stacks;
} stack_list = {0U, NULL};

int find_stack_idx(const hstack_t handler)
{
    // Функция поиска индекса искомого стэка в списке
    for (size_t i = 0; i < stack_list.size; ++i)
    {
        if (stack_list.stacks[i].stack_handler == handler)
	{
            return (int)i;
	}
    }
    return -1;
}

hstack_t stack_new(void)
{
    // Выделение памяти под стэк
    stack_list.stacks = (stack_type*)realloc(stack_list.stacks, 
        ++stack_list.size * sizeof(stack_type));
    stack_type *new_stack = &stack_list.stacks[stack_list.size - 1];
    hstack_t valid_handler = 1;
    while (!stack_valid_handler(valid_handler))
    {
	// Поиск свободного хэндлера
        ++valid_handler;
    }
    new_stack->stack_handler = valid_handler;
    new_stack->head = NULL;
    return new_stack->stack_handler;
}

void stack_free(const hstack_t hstack)
{
    int stack_idx = find_stack_idx(hstack);
    if (stack_idx == -1) return;
    while (stack_list.stacks[stack_idx].head)
    {
	// Очистка стэка от всех элементов, если таковые есть
        void* data_out = malloc(stack_list.stacks[stack_idx].head->data_size);
        stack_pop(hstack, data_out, stack_list.stacks[stack_idx].head->data_size);
	free(data_out);
    }
    if (stack_idx != (int)stack_list.size - 1)
    {
	// Перенос на место удаляемого стэка конечного
	stack_list.stacks[stack_idx] = stack_list.stacks[stack_list.size - 1];
    }
    // Очистка памяти последнего элемента
    stack_list.stacks = (stack_type*)realloc(stack_list.stacks,
        --stack_list.size * sizeof(stack_type));
}

int stack_valid_handler(const hstack_t hstack)
{
    if (find_stack_idx(hstack) != -1) 
        return 0;
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    int stack_idx = find_stack_idx(hstack);
    if (stack_idx == -1) return 0U;
    unsigned int sum_size = 0U;
    if (!stack_list.stacks[stack_idx].head) return 0U;
    stack_item_t* head = stack_list.stacks[stack_idx].head;
    while (head)
    {
        ++sum_size;
        head = (stack_item_t*)head->prev_item;
    }
    return sum_size;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    int stack_idx = find_stack_idx(hstack);
    if (stack_idx == -1 || !data_in || !size) return;
    // Выделение памяти под элемент стэка
    stack_item_t *new_stack_item = (stack_item_t*)malloc(sizeof(stack_item_t));
    new_stack_item->prev_item = (stack_item_t*)stack_list.stacks[stack_idx].head;
    new_stack_item->data_size = (size_t)size;
    // Выделение памяти под данные
    new_stack_item->data = malloc(size);
    memmove(new_stack_item->data, data_in, size);
    stack_list.stacks[stack_idx].head = new_stack_item;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    int stack_idx = find_stack_idx(hstack);
    if (stack_idx == -1) return 0U;
    if (!stack_list.stacks[stack_idx].head) return 0U;
    if (size < stack_list.stacks[stack_idx].head->data_size) return 0U;
    if (!data_out) return 0U;
    memmove(data_out, stack_list.stacks[stack_idx].head->data, size);
    stack_item_t* to_free = stack_list.stacks[stack_idx].head;
    stack_list.stacks[stack_idx].head = (stack_item_t*)to_free->prev_item;
    // Освобождение памяти в месте хранения данных
    free(to_free->data);
    // Освобождение памяти в месте хранения элемента стэка
    free(to_free);
    return size;
}


#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct item
{
    unsigned int size;
    char* data;
    struct item *prev;
} item;

typedef struct stack_list
{
    int size;
    int capacity;
    item** stack;
} stack_list_;

stack_list_ stack_list = {.size = 0, .capacity = 10, .stack = NULL};

static int is_initialized = 0;//за такое надо банить

static void stack_list_init() {
    if (!is_initialized){
        stack_list.stack = (item**)calloc(stack_list.capacity,sizeof(item*));
        is_initialized =1;
    }
    
}

hstack_t stack_new(void)
{
    if (stack_list.size >= stack_list.capacity) {
        stack_list.capacity *= 2;
        stack_list.stack = (item**)realloc(stack_list.stack, stack_list.capacity * sizeof(item*));
        if (!stack_list.stack) return -1;
    }
    stack_list.stack[stack_list.size] = NULL;
    return stack_list.size++;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack)) {
        item* current = stack_list.stack[hstack];
        while (current) {
            item* prev = current->prev;
            free(current->data);
            free(current);
            current = prev;
        }
        stack_list.stack[hstack] = NULL;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    return (hstack >= 0 && hstack < stack_list.size 
            && stack_list.stack[hstack] != NULL);
}

unsigned int stack_size(const hstack_t hstack)
{
    if (stack_valid_handler(hstack)) {
        unsigned int size = 0;
        item* current = stack_list.stack[hstack];
        while (current) {
            size++;
            current = current->prev;
        }
        return size;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (!data_in || size == 0 || !stack_valid_handler(hstack))//проверка входных данных и хендлер
        return;
    item* new_item = (item*)malloc(sizeof(item));
    if (!new_item) return;

    new_item->data = (char*)malloc(size);
    if (!new_item->data) {
        free(new_item);
        return;
    }

    for (unsigned int i = 0; i < size; i++) {
        new_item->data[i] = ((char*)data_in)[i];
    }

    new_item->size = size;
    new_item->prev = stack_list.stack[hstack];
    stack_list.stack[hstack] = new_item;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (!data_out || size == 0 || !stack_valid_handler(hstack)) return 0;

    item* top = stack_list.stack[hstack];
    if (!top || top->size != size) return 0;

    for (unsigned int i = 0; i < size; i++) {
        ((char*)data_out)[i] = top->data[i];
    }

    stack_list.stack[hstack] = top->prev;
    free(top->data);
    free(top);
    return size;
}

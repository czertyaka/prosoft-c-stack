#ifndef CSTACK_H
#define CSTACK_H
#include <stdlib.h>

typedef int hstack_t;

struct stack_item
{
    const struct stack_item* prev_item;
    void *data;
    size_t data_size;
};

typedef struct stack_item stack_item_t;

struct stack
{
    stack_item_t* head;
    hstack_t stack_handler;
};

typedef struct stack stack_type;


hstack_t stack_new(void);
void stack_free(const hstack_t stack);
int stack_valid_handler(const hstack_t stack);
unsigned int stack_size(const hstack_t stack);
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

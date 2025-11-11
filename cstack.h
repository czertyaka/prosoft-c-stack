#ifndef CSTACK_H
#define CSTACK_H

#include <stdlib.h>

typedef int hstack_t;

struct stackElem_t
{
    struct stackElem_t* prev;
    unsigned int size;
    char data[0];
};

struct stackB_t
{
    struct stackB_t* prev;
    struct stackElem_t* stackLast;
    hstack_t hstack;
};

static struct stackB_t* laststack = NULL;

struct stackB_t* stackB_search(const hstack_t hstack);
void freeElem(struct stackElem_t* stackElem);

hstack_t stack_new(void);
void stack_free(const hstack_t stack);
int stack_valid_handler(const hstack_t stack);
unsigned int stack_size(const hstack_t stack);
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

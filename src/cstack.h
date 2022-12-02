#ifndef CSTACK_H
#define CSTACK_H

typedef struct node_t node;
typedef int hstack_t;

struct node_t
{
    const node* prev;
    void* data;
    unsigned int size;
};



hstack_t stack_new();
void stack_free(const hstack_t stack);
int stack_valid_handler(const hstack_t stack);
unsigned int stack_size(const hstack_t stack);
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

hstack_t staks;
int entries = 0;

#endif /* CSTACK_H */

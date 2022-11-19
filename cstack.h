#ifndef CSTACK_H
#define CSTACK_H

struct node
{
    struct node* prev;
    const unsigned int size;
    void* data;
};

typedef struct node node_t;
typedef const node_t* cstack_t;

cstack_t stack_new();
void stack_free(cstack_t* stack);
unsigned int stack_size(cstack_t* stack);
void stack_push(cstack_t* stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(cstack_t* stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

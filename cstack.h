#ifndef CSTACK_H
#define CSTACK_H

struct node
{
    void* data;
    const unsigned int size;
    struct node* next;
};

typedef struct node node_t;
typedef node_t* cstack_t;

cstack_t* stack_new();
void stack_free(cstack_t* list);
unsigned int stack_size(cstack_t* stack);
void stack_push(cstack_t* stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(cstack_t* stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

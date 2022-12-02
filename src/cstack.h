#ifndef CSTACK_H
#define CSTACK_H

#define _MAX_STACKS 16

typedef int hstack_t;
 
typedef struct node_t node;
struct node_t
{
    node* prev;
    void* data;
    unsigned int size;
};


typedef struct stack_head_t stack_head;
struct stack_head_t
{
    node* head_pointer ;
    unsigned int size ;
    int is_free ;
};


hstack_t stack_new();
void stack_free(const hstack_t stack);
int stack_valid_handler(const hstack_t stack);
unsigned int stack_size(const hstack_t stack);
int stack_push(const hstack_t stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

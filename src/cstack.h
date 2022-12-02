#ifndef CSTACK_H
#define CSTACK_H

#define _MAX_STACKS 16

typedef int hstack_id;
 
typedef struct node_t node;
struct node_t
{
    const node* prev;
    void* data;
    unsigned int size;
};

typedef int bool;
enum { false, true };

typedef struct stack_head_t stack_head;
struct stack_head_t
{
    node* head_pointer;
    unsigned int size;
    bool is_free;
};


stack_head stacks_head[_MAX_STACKS] = { ((void*)0), 0, true };

hstack_id stack_new();
void stack_free(const hstack_id stack);
int stack_valid_handler(const hstack_id stack);
unsigned int stack_size(const hstack_id stack);
int stack_push(const hstack_id stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_id stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

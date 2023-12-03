#ifndef CSTACK_H
#define CSTACK_H

typedef int hstack_t;

struct node {
    struct node* prev;
    unsigned int data_size;
    void* p_data;
};

struct stack_head {

    hstack_t id;
    struct stack_head* p_prev;
    unsigned int stack_size;
    struct node* p_node;
};


hstack_t stack_new(void);
void stack_free(const hstack_t stack);
int stack_valid_handler(const hstack_t stack);
unsigned int stack_size(const hstack_t stack);
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

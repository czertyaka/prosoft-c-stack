#ifndef CSTACK_H
#define CSTACK_H

void* stack_new();
void stack_free(void* stack);
unsigned int stack_size(void* stack);
void stack_push(void* stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(void* stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

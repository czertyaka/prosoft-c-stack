#include "cstack.h"
#include <stddef.h>

#define UNUSED(VAR) (void)(VAR)
static int stack_is_freed = 1;

hstack_t stack_new(void)
{
    stack_is_freed = 0; // создаём стек 0
    return 0;
}

void stack_free(const hstack_t hstack)
{
    if (hstack != 0) {
        return; // свободный хэндл игнорируем
    }

    stack_is_freed = 1;
}

int stack_valid_handler(const hstack_t hstack)
{
    if (hstack != 0) {
        return 1; // -1 или что угодно → невалидно
    }
    return stack_is_freed ? 1 : 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    
    if (stack_valid_handler(hstack)) {
        return 0;
    }

    return 0; 
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack)) {
        return;
    }
    if (!data_in || size == 0u) {
        return;
    }
    UNUSED(size);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    
    if (stack_valid_handler(hstack)) {
        return 0;
    }

    if (!data_out || size == 0u) {
        return 0;
    }

    return 0; // стек пустой → возвращаем 0
}


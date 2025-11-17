#include "cstack.h"
#include <stddef.h>
.....
#define UNUSED(VAR) (void)(VAR)

hstack_t stack_new(void)
{
    return 0 ;
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack) != 0) {
        return;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    return (hstack < 0) ? 1 : 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    
    if (stack_valid_handler(hstack) != 0) {
        return 0;
    }
    
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack) != 0) {
        return;
    }
    if (!data_in || size == 0u) {
        return;
    }
    UNUSED(size);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    
    if (stack_valid_handler(hstack) != 0) {
        return 0;
    }

   
    if (!data_out || size == 0u) {
        return 0;
    }

    
    return 0;
}


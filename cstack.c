#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>

#define NSTACKS ( 8 )
#define STACK_MAX_LENGTH ( 32 )
#define STACK_INVALID_HANDLE ( -1 )

#ifndef NDEBUG
#include <stdio.h>
#endif

struct stack {
    char* base;
    char* top;

    int empty;
};

struct {
    struct stack* table[NSTACKS];
    size_t append_index;

} stack_table = { .table = {}, .append_index = 0u };

hstack_t stack_new( void ) {
    struct stack* const stack = malloc( sizeof( struct stack ) );

    if ( !stack ) {
        return STACK_INVALID_HANDLE;
    }

    stack->base = calloc( 1, STACK_MAX_LENGTH );
    stack->top = stack->base;
    stack->empty = 1;

    stack_table.table[stack_table.append_index] = stack;

    return stack_table.append_index++;
}

void stack_free( const hstack_t hstack ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return;
    }

    struct stack* stack = stack_table.table[hstack];

    free( stack->base );
    free( stack );

    stack_table.table[hstack] = 0;
}

int stack_valid_handler( const hstack_t hstack ) {

    if (hstack >= 0 && hstack < stack_table.append_index) {
        return 0;
    }

    return 1;
}

unsigned int stack_size( const hstack_t hstack ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return 0u;
    }

    struct stack* stack = stack_table.table[hstack];

    if (stack->empty) {
        return 0;
    }

    return ( stack->top - stack->base + 1 );
}

void stack_push( const hstack_t hstack, const void* buffer,
                 const unsigned int bflen ) {

    if ( !buffer || !bflen || 0 != stack_valid_handler( hstack ) ) {
        return;
    }

    struct stack* stack = stack_table.table[hstack];
    char* source = (char*)buffer;

    for ( size_t i = 0u; i < bflen; i++ ) {

        stack->top[i] = source[i];
        printf( "%i\n", stack->top[i] );
    }

    stack->top += bflen - 1;
    stack->empty = 0;
}

unsigned int stack_pop( const hstack_t hstack, void* buffer,
                        const unsigned int bflen ) {

    if ( !buffer || !bflen || 0 != stack_valid_handler( hstack ) ) {
        return 0u;
    }

    struct stack* const stack = stack_table.table[hstack];

    const size_t stacklen = stack->top - stack->base + 1u;
    size_t nwrbytes = bflen;

    if ( bflen > stacklen ) {
        nwrbytes = stacklen;
    }

    char* const destination = buffer;

    for ( size_t i = 0u; i < nwrbytes; i++ ) {
        printf( "%d\n", *( stack->top ) );

        destination[i] = *( stack->top );
        --stack->top;
    }

    if (-1 == stack->top - stack->base) {

        stack->empty = 1;
        stack->top = stack->base;
    }

    return nwrbytes;
}

#ifndef NDEBUG
void stack_print( const hstack_t hstack ) {

    const struct stack* const stack = stack_table.table[hstack];
    const size_t size = stack->top - stack->base + 1u;

    for ( size_t i = 0u; i < size; i++ ) {
        printf( "%d\n", stack->base[i] );
    }
}
#endif

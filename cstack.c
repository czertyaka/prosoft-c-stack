#include "cstack.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define NSTACKS ( 16 )
#define STACK_MAX_LENGTH ( 32 )
#define STACK_INVALID_HANDLE ( -1 )

#ifndef NDEBUG
#include <stdio.h>
#endif

struct stack {
    uint8_t* base;
    uint8_t* top;

    size_t npush;
    int empty;
};

struct {
    struct stack* table[NSTACKS];

} stack_table = { .table = {} };

hstack_t stack_new( void ) {
    size_t first_free = NSTACKS;

    for (size_t i = 0u; i < NSTACKS; i++) {

        if (stack_table.table[i] == 0) {
            first_free = i;
            break;
        }
    }

    if (first_free == NSTACKS) {
        return STACK_INVALID_HANDLE;
    }

    struct stack* const stack = malloc( sizeof( struct stack ) );

    if ( !stack ) {
        return STACK_INVALID_HANDLE;
    }

    stack->base = calloc( 1, STACK_MAX_LENGTH );
    stack->top = stack->base;
    stack->empty = 1;
    stack->npush = 0u;

    /* printf("%lu %p\n", first_free, stack); */
    stack_table.table[first_free] = stack;

    return first_free;
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

    if ( hstack >= NSTACKS ) {
        return 1;
    }

    if (stack_table.table[hstack] == 0) {
        return 1;
    }

    return 0;
}

unsigned int stack_size( const hstack_t hstack ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return 0u;
    }

    struct stack* stack = stack_table.table[hstack];

    return stack->npush;

    /* if ( stack->empty ) { */
        /* return 0; */
    /* } */

    /* return ( stack->top - stack->base + 1 ); */
}

void stack_push( const hstack_t hstack, const void* buffer,
                 const unsigned int bflen ) {

    if ( !buffer || !bflen || 0 != stack_valid_handler( hstack ) ) {
        return;
    }

    struct stack* const stack = stack_table.table[hstack];
    const uint8_t* const source = (uint8_t*)buffer;

    /* printf("%x %x\n", stack->base, stack->top); */

    for ( size_t i = 0u; i < bflen; i++ ) {

        stack->top[i] = source[i];
        printf( "%i\n", stack->top[i] );
    }

    stack->top += bflen - 1;
    stack->empty = 0;
    (stack->npush)++;
}

unsigned int stack_pop( const hstack_t hstack, void* buffer,
                        const unsigned int bflen ) {

    if (0 != stack_valid_handler( hstack )) {
        return 0u;
    }

    if ( !buffer || !bflen) {
        return 0u;
    }

    struct stack* const stack = stack_table.table[hstack];

    if (0u == stack->npush) {
        return 0u;
    }

    const size_t stacklen = stack->top - stack->base + 1u;
    size_t nwrbytes = bflen;

    if ( bflen > stacklen ) {
        nwrbytes = stacklen;
    }

    uint8_t* const destination = (uint8_t*)buffer;
    uint8_t *start = stack->base + (stacklen - nwrbytes);

    for ( size_t i = 0u; i < nwrbytes; i++ ) {
        printf( "%d\n", *( start + i ));

        destination[i] = *( start + i );
    }

    stack->top -= nwrbytes;
    (stack->npush)--;

    if ( -1 == stack->top - stack->base ) {

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

#ifndef NDEBUG
void stack_table_print( void ) {

    for (size_t i = 0; i < NSTACKS; i++) {
        printf("%p\n", stack_table.table[i]);
    }
}
#endif

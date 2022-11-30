#include "cstack.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    /* int empty; */

    /* element size table */
    /* NO -> nbytes */
    size_t* elst;
};

struct {
    struct stack* table[NSTACKS];

} stack_table = { .table = {} };

hstack_t stack_new( void ) {
    size_t first_free = NSTACKS;

    for ( size_t i = 0u; i < NSTACKS; i++ ) {

        if ( stack_table.table[i] == 0 ) {
            first_free = i;
            break;
        }
    }

    if ( NSTACKS == first_free ) {
        return STACK_INVALID_HANDLE;
    }

    struct stack* const stack = malloc( sizeof( struct stack ) );

    if ( !stack ) {
        return STACK_INVALID_HANDLE;
    }

    stack->base = calloc( 1, STACK_MAX_LENGTH );

    if (NULL == stack->base) {
        return STACK_INVALID_HANDLE;
    }

    stack->elst = calloc( STACK_MAX_LENGTH, sizeof( size_t ) );

    if (NULL == stack->elst) {

        free(stack->base);
        return STACK_INVALID_HANDLE;
    }

    stack->top = 0;
    stack->npush = 0u;
    stack_table.table[first_free] = stack;

    return first_free;
}

void stack_free( const hstack_t hstack ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return;
    }

    struct stack* stack = stack_table.table[hstack];

    free( stack->base );
    free( stack->elst );
    free( stack );

    stack_table.table[hstack] = 0;
}

int stack_valid_handler( const hstack_t hstack ) {

    if ( hstack >= NSTACKS ) {
        return 1;
    }

    if ( stack_table.table[hstack] == 0 ) {
        return 1;
    }

    return 0;
}

unsigned int stack_size( const hstack_t hstack ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return 0u;
    }

    return stack_table.table[hstack]->npush;
}

void stack_push( const hstack_t hstack, const void* buffer,
                 const unsigned int bflen ) {

    if ( !buffer || !bflen || 0 != stack_valid_handler( hstack ) ) {
        return;
    }

    struct stack* const stack = stack_table.table[hstack];
    const uint8_t* const source = (uint8_t* const)buffer;

    if ( stack->npush == 0u ) {
        stack->top = stack->base;
    }

    /* printf("%x %x\n", stack->base, stack->top); */

    for ( size_t i = 0u; i < bflen; i++ ) {

        stack->top[i] = source[i];
        printf( "%i\n", stack->top[i] );
    }

    stack->top += bflen - (stack->npush == 0u);

    printf("%u %lu\n", bflen, stack->top - stack->base + 1);
    stack->elst[stack->npush] = bflen;

    /* stack->empty = 0; */
    ( stack->npush )++;
}

unsigned int stack_pop( const hstack_t hstack, void* buffer,
                        const unsigned int bflen ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return 0u;
    }

    if ( !buffer || !bflen ) {
        return 0u;
    }

    struct stack* const stack = stack_table.table[hstack];

    if ( 0u == stack->npush ) {
        return 0u;
    }

    if ( stack->elst[stack->npush - 1] > bflen ) {
        return 0u;
    }

    const size_t stacklen = stack->top - stack->base + 1u;
    size_t nwrbytes = bflen;

    if ( bflen > stacklen ) {
        nwrbytes = stacklen;
    }

    printf("%lu %u %lu\n", stacklen, bflen, stack->npush);

    uint8_t* const destination = (uint8_t* const)buffer;
    uint8_t* current = stack->top - nwrbytes;

    if (current == stack->base) {
        current++;
    }

    int base_catched = 0;

    for ( size_t i = 0u; i < nwrbytes;  ) {

        printf("%i %i\n", *current, current + 1 == stack->base);

        if (current + 1 == stack->base) {
            base_catched=1;
        }

        destination[i - base_catched] = *current;

        current++;
        i++;
    }

    stack->top -= nwrbytes;
    ( stack->npush )--;

    //TODO обновление elst

    /* if ( -1 == stack->top - stack->base ) { */

    /* stack->empty = 1; */
    /* stack->top = stack->base; */
    /* } */

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

    for ( size_t i = 0u; i < NSTACKS; i++ ) {
        printf( "%p\n", stack_table.table[i] );
    }
}
#endif

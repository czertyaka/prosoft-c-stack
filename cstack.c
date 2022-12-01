#include "cstack.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef NDEBUG
#include <stdio.h>
#endif

struct stack {

    /* адрес базы стека */
    uint8_t* base;

    /* адрес вершины стека */
    uint8_t* top;

    /* количествово элементов (не байтов!) в стеке */
    size_t npush;

    /* таблица размеров элементов */
    /* ставит в соответствие элементу его размер в байтах */
    /* (el)ement (s)ize (t)able */
    size_t* elst;
};

struct {
    struct stack* table[NSTACKS_MAX];

} stack_table = { .table = NULL };

hstack_t stack_new( void ) {
    size_t nfirst_free = NSTACKS_MAX;

    for ( size_t i = 0u; i < NSTACKS_MAX; i++ ) {

        if ( NULL == stack_table.table[i] ) {
            nfirst_free = i;
            break;
        }
    }

    if ( NSTACKS_MAX == nfirst_free ) {
        return STACK_INVALID_HANDLE;
    }

    struct stack* const stack = malloc( sizeof( struct stack ) );

    if ( NULL == stack ) {
        return STACK_INVALID_HANDLE;
    }

    stack->base = calloc( 1, STACK_MAX_SIZE );

    if ( NULL == stack->base ) {
        free( stack );

        return STACK_INVALID_HANDLE;
    }

    stack->elst = calloc( STACK_MAX_SIZE, sizeof( size_t ) );

    if ( NULL == stack->elst ) {
        free( stack->base );
        free( stack );

        return STACK_INVALID_HANDLE;
    }

    stack->top = stack->base;
    stack->npush = 0u;
    stack_table.table[nfirst_free] = stack;

    return nfirst_free;
}

void stack_free( const hstack_t hstack ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return;
    }

    struct stack* const stack = stack_table.table[hstack];

    free( stack->base );
    free( stack->elst );
    free( stack );

    stack_table.table[hstack] = NULL;
}

int stack_valid_handler( const hstack_t hstack ) {

    if ( NSTACKS_MAX <= (unsigned)hstack ) {
        return 1;
    }

    if ( NULL == stack_table.table[hstack] ) {
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
                 const unsigned int bfsize ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return;
    }

    if ( NULL == buffer || 0u == bfsize ) {
        return;
    }

    if ( bfsize > STACK_MAX_SIZE ) {
        return;
    }

    const size_t nfreebytes = STACK_MAX_SIZE - stack_size( hstack );

    if ( nfreebytes < bfsize ) {
        return;
    }

    struct stack* const stack = stack_table.table[hstack];
    const uint8_t* const source = buffer;

    uint8_t* push_start = stack->top + ( stack->top != stack->base );
    memcpy( push_start, source, bfsize );

    stack->top += bfsize - ( stack->top == stack->base );
    stack->elst[stack->npush++] = bfsize;
}

unsigned int stack_pop( const hstack_t hstack, void* buffer,
                        const unsigned int bfsize ) {

    if ( 0 != stack_valid_handler( hstack ) ) {
        return 0u;
    }

    if ( NULL == buffer || 0u == bfsize ) {
        return 0u;
    }

    struct stack* const stack = stack_table.table[hstack];

    if ( 0u == stack->npush ) {
        return 0u;
    }

    const size_t elsize = stack->elst[stack->npush - 1];

    if ( elsize > bfsize ) {
        return 0u;
    }

    const uint8_t* const start = stack->top - elsize + 1;
    memcpy( buffer, start, bfsize );

    stack->top -= elsize;
    stack->elst[--stack->npush] = 0u;

    return elsize;
}

#ifndef NDEBUG
void stack_print( const hstack_t hstack ) {

    const struct stack* const stack = stack_table.table[hstack];
    const size_t size = stack_size( hstack );

    for ( size_t i = 0u; i < size; i++ ) {
        printf( "%x\n", stack->base[i] );
    }
}
#endif

#ifndef NDEBUG
void stack_table_print( void ) {

    for ( size_t i = 0u; i < NSTACKS_MAX; i++ ) {
        printf( "%p\n", (void*)stack_table.table[i] );
    }
}
#endif

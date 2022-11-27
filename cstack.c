#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
};

/*struct stack_entry
{
    int reserved;
    stack_t stack;
};*/

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};

int stacks_count = 0;
struct node** p_top_nodes; 

hstack_t stack_new()
{
    hstack_t handler = -1;

    // search for empty space
    for (int i = 0; i < stacks_count; i++){ 
        if (p_top_nodes[i] == NULL) {
            //printf("cell %d is empty ", i);
            p_top_nodes[i] = malloc(sizeof(struct{}*));
            if (p_top_nodes[i] == NULL) { //checking for correct memory allocation
                return -1;
            }
            handler = i;
            return handler;
        }
    }
    // array extension
    p_top_nodes = realloc(p_top_nodes, (stacks_count + 1) * sizeof(struct{}**));
    if (p_top_nodes == NULL) { //checking for correct memory allocation
        return -1;
    }
    p_top_nodes[stacks_count] = malloc(sizeof(struct{}*)); // creating a new pointer to a top node
    if (p_top_nodes [stacks_count - 1] == NULL) { //checking for correct memory allocation
        return -1;
    }
    stacks_count += 1; 
    handler = stacks_count - 1;
    
    return handler;
}


//////////////////

void stack_free(const hstack_t hstack)
{
    UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{
    UNUSED(hstack);
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    UNUSED(hstack);
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_in);
    UNUSED(size);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_out);
    UNUSED(size);
    return 0;
}

int main()
{
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    printf("%d\n", stack_new());
    
    return 0;
}
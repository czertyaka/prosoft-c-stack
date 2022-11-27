#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    const struct node* prev;
    unsigned int size;
    char data;
    unsigned int node_number;
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

// Functions:

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

void stack_free(const hstack_t hstack)
{
    UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{
    //debug start
    /*printf("stack_valid_handler called\n");
    if (hstack >= 0) printf("hstack >= 0\n");
    printf("stacks_count = %d\n", stacks_count);
    printf("hstack = %d\n", hstack);
    if (hstack < stacks_count) printf("hstack < stacks_count\n");
    if (p_top_nodes[hstack] != NULL) printf("p_top_nodes[hstack] != NULL\n");
    *///debug end
    if ((hstack >= 0) && (hstack < stacks_count) && (p_top_nodes[hstack] != NULL)){
        return 0;
    } else return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    UNUSED(hstack);
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    struct node* p_prev = p_top_nodes[hstack]; //saving pointer to prev element 
    int new_node_number =  p_top_nodes[hstack] -> node_number + 1; 
    p_top_nodes[hstack] = malloc(sizeof(struct{}*));
    /*if (p_top_nodes [hstack] == NULL) { //checking for correct memory allocation
    // Добавить обработку некорректного выделения памяти!!!
    }*/
    p_top_nodes[hstack] -> node_number = new_node_number;
    p_top_nodes[hstack] -> size = size;
    p_top_nodes[hstack] -> data = malloc (size);
    // Добавить обработку некорректного выделения памяти!!!
    void *desptr = p_top_nodes[hstack] -> data;
    memcpy(desptr, data_in, size);
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
    printf("Enter the handler: \n");
    int h;
    scanf("%d", &h);
    if (!stack_valid_handler(h)) {
        printf("ptr is valid\n");
    } else printf("ptr is NOT valid\n");
    return 0;
}
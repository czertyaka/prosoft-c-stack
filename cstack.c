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
    char* data;
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
/*struct top_node_table {
    struct node* p_top_nodes
};*/

int stacks_count = 0;
struct node** p_top_nodes; 

// Functions:

hstack_t stack_new()
{
    hstack_t handler = -1;

    // search for empty space
    //как не путать свободное место от дна стека???
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
    
}

int stack_valid_handler(const hstack_t hstack)
{
    if ((hstack >= 0) && (hstack < stacks_count) && (p_top_nodes[hstack] != NULL)){
        return 0;
    } else return 1;
}

unsigned int stack_size(const hstack_t hstack)
{   
    if (stack_valid_handler(hstack) == 0){
        if (p_top_nodes[hstack] != NULL){
        unsigned int size = p_top_nodes[hstack] -> node_number;
        return size;
        }
        else return 0;
    }
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if ((stack_valid_handler(hstack) == 0)&&(data_in != NULL)){
        struct node* p_prev = p_top_nodes[hstack]; //saving pointer to prev element 
        int new_node_number =  p_top_nodes[hstack] -> node_number + 1; 
        p_top_nodes[hstack] = malloc(sizeof(struct{}*));
        /*if (p_top_nodes [hstack] == NULL) { //checking for correct memory allocation
        // Добавить обработку некорректного выделения памяти!!!
        }*/
        p_top_nodes[hstack] -> node_number = new_node_number;
        p_top_nodes[hstack] -> size = size;
        p_top_nodes[hstack] -> data = (char*) malloc (size);
        // Добавить обработку некорректного выделения памяти!!!
        char *desptr = p_top_nodes[hstack] -> data;
        memcpy(desptr, data_in, size);
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    printf("pop called\n");
    printf("hstack = %d\n", hstack);
    if ((stack_valid_handler(hstack) == 0)
        &&(data_out != NULL)
        &&(p_top_nodes[hstack] != NULL))
    {  
        printf("conditions is OK\n");
        int size_to_copy = (p_top_nodes[hstack] -> size > size) ? size : p_top_nodes[hstack] -> size;
        printf("size_to_copy = %d\n", size_to_copy);
        char *desptr = p_top_nodes[hstack] -> data;
        memcpy(data_out, desptr, size_to_copy);
        printf("Copied sucsessful\n");
        const struct node* prev = p_top_nodes[hstack] -> prev;
        printf("prev is seted\n");
        //ДОРАБОТАТЬ ОЧИЩЕНИЕ ПАМЯТИ!!!!!!!!!!!!!!
        //free(p_top_nodes[hstack] -> data); 
        //free(p_top_nodes[hstack]);
        p_top_nodes[hstack] = prev;
        printf("end of pop\n\n");
        return size_to_copy;
    }
}

int main()
{   //stack_new test
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

    //stack valid handler and push test

    int x;
    printf("Enter the int to push: \n");
    scanf("%d", &x);

    printf("Enter the handler: \n");
    int h;
    scanf("%d", &h);
    if (!stack_valid_handler(h)) {
        printf("Handler is valid\n");
    } else printf("Handler is NOT valid\n");

    //stack push test    
    stack_push(h, &x, sizeof(x));
    printf("stack_push called\n");
    /*char *p_c = p_top_nodes[h] -> data;
    char c = *p_c;
    printf("Pushed: %c\n", c);*/

    // stack pop test
    int y;
    stack_pop(h, &y, 4);
    printf("Popped: %d\n", y);
    return 0;
}
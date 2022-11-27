#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

typedef struct node
{
    const struct node* prev;
    unsigned int size;
    char* data;
    unsigned int node_number;
} node_t;

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


//////////new achitecture
typedef struct top_node_table {
    unsigned short int reserved;
    struct node* p_top_node;
} stack_t;

stack_t* handlers_table; 
int stacks_count = 0;

// Functions:

hstack_t stack_new()
{
    hstack_t handler = -1;
    printf("stacks_count = %d\n", stacks_count);
    // search for empty space
    for (int i = 0; i < stacks_count; i++) {
        if (handlers_table[i].reserved == 0) {
            printf("Cell %d is empty\n", i);
            handlers_table[i].reserved = 1;
            handlers_table[i].p_top_node = NULL;
            return i;
        }
    }
    // array extension
    handlers_table = (stack_t*) realloc(handlers_table, (stacks_count + 1) * sizeof(stack_t*));
    if (handlers_table == NULL) { //checking for correct memory allocation
        printf("Allocation error\n");
        return -1;
    }
    ++ stacks_count;
    handler = stacks_count - 1;
    handlers_table[handler].reserved = 1;
    handlers_table[handler].p_top_node = NULL;
    printf("Array extension, returned %d\n", handler);
    return handler;
}

void stack_free(const hstack_t hstack) {
    if (stack_valid_handler(hstack) != 0) {
        return;
    }
    if (handlers_table[hstack].p_top_node == NULL) {
        handlers_table[hstack].reserved = 0;
        return;
    } //добавить дефрагментацию
    while (handlers_table[hstack].p_top_node != NULL) {
        const struct node* prev_node = handlers_table[hstack].p_top_node -> prev;
        //НЕ РАБОТАЕТ ОСВОБОЖДЕНИЕ ПАМЯТИ!!!!!!!!!!!!!!
        //free(handlers_table[hstack].p_top_node -> data); 
        //free(handlers_table[hstack].p_top_node);
        handlers_table[hstack].p_top_node = prev_node;
    }
    handlers_table[hstack].reserved = 0;
}   

int stack_valid_handler(const hstack_t hstack){
    if ((hstack >= 0) && (hstack < stacks_count) && (handlers_table[hstack].reserved != 0)){
        return 0;
    } else return 1;
}

unsigned int stack_size(const hstack_t hstack){   
    if (stack_valid_handler(hstack) == 0){
        if (handlers_table[hstack].p_top_node != NULL){
            unsigned int size = handlers_table[hstack].p_top_node -> node_number;
            return size;
        }
        else return 0;
    }
}

//НУЖНО СДЕЛАТЬ ЧИТАБЕЛЬНЕЕ!!!

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size) {     
    if ((stack_valid_handler(hstack) == 0)&&(data_in != NULL)){
        struct node* p_prev = handlers_table[hstack].p_top_node; //saving pointer to prev element
        int new_node_number = 1;
        if (p_prev != NULL) {
            new_node_number = p_prev -> node_number + 1; //increment number of node
        }
        handlers_table[hstack].p_top_node = (node_t*) malloc(sizeof(node_t*));
        if (handlers_table[hstack].p_top_node == NULL) { //checking for correct memory allocation
            return;
        }
        handlers_table[hstack].p_top_node -> prev = p_prev;
        handlers_table[hstack].p_top_node -> node_number = new_node_number;
        handlers_table[hstack].p_top_node -> data = (char*) malloc (size);
        if (handlers_table[hstack].p_top_node -> data == NULL) { //checking for correct memory allocation
            return;
        }
        handlers_table[hstack].p_top_node -> size = size;
        char *data_out = handlers_table[hstack].p_top_node -> data;
        memcpy(data_out, data_in, size);
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    printf("pop called\n");
    printf("hstack = %d\n", hstack);
    if ((stack_valid_handler(hstack) == 0)
        &&(data_out != NULL)
        &&(handlers_table[hstack].p_top_node != NULL)) {

        printf("conditions is OK\n");
        int size_to_copy = (handlers_table[hstack].p_top_node -> size > size) ? size : handlers_table[hstack].p_top_node -> size;
        printf("size_to_copy = %d\n", size_to_copy);
        char *data_in = handlers_table[hstack].p_top_node -> data;
        memcpy(data_out, data_in, size_to_copy);
        printf("Copied sucsessful\n");
        const struct node* prev_node = handlers_table[hstack].p_top_node -> prev;
        printf("prev is seted\n");
        //НЕ РАБОТАЕТ ОСВОБОЖДЕНИЕ ПАМЯТИ!!!!!!!!!!!!!!
        //free(handlers_table[hstack].p_top_node -> data); 
        //free(handlers_table[hstack].p_top_node);
        handlers_table[hstack].p_top_node = prev_node;
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

    // stack pop test
    int y;
    stack_pop(h, &y, 4);
    printf("Popped: %d\n", y);
    return 0;
}
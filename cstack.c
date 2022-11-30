#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    const struct node* prev;
    unsigned int size;
    char* data;
    unsigned int node_number;
} node_t;

typedef struct top_node_table {
    unsigned short int reserved;
    struct node* p_top_node;
} stack_t;

stack_t** handlers_table; 
int table_size = 0;

//Как сделать код читабельнее?

// Functions:

hstack_t stack_new(){
    hstack_t handler = -1;
    // search for empty space
    for (int i = 0; i < table_size; i++) {
        if (handlers_table[i] -> reserved == 0) {
            handlers_table[i] -> reserved = 1;
            handlers_table[i] -> p_top_node = NULL;
            return i;
        }
    }
    // array extension
    handlers_table = realloc(handlers_table, (table_size + 1) * sizeof(handlers_table));
    if (handlers_table == NULL) { //checking for correct memory allocation
        return -1;
    }
    ++ table_size;
    handler = table_size - 1;
    handlers_table[handler] = malloc(sizeof(stack_t*));
    handlers_table[handler] -> reserved = 1;
    handlers_table[handler] -> p_top_node = NULL;
    return handler;
}

void stack_free(const hstack_t hstack) {
    //checking for valid memory
    if (stack_valid_handler(hstack) == 1) {
        return;
    }
    //checking for 0 element
    if (handlers_table[hstack] -> p_top_node == NULL) {
        handlers_table[hstack] -> reserved = 0;
        return;
    }
    //delete all elements
    while (handlers_table[hstack] -> p_top_node != NULL) {
        const node_t* prev_node = handlers_table[hstack] -> p_top_node -> prev;
        //Правильно ли освобождаю память?
        free(handlers_table[hstack] -> p_top_node -> data); 
        free(handlers_table[hstack] -> p_top_node);
        handlers_table[hstack] -> p_top_node = prev_node;
    }
    handlers_table[hstack] -> reserved = 0;
    //resize table if able
    unsigned int size_to_reduce = 0; //count how much we can reduce
    for (int i = table_size - 1; ((i >= 0)||(handlers_table[i] -> reserved == 0)); i--){
        ++ size_to_reduce;
    }
    //decrease
    if (size_to_reduce) {
        handlers_table = realloc(handlers_table, (table_size - size_to_reduce) * sizeof(handlers_table));
    }
    --table_size;
}   

int stack_valid_handler(const hstack_t hstack){
    if ((hstack >= 0) && (hstack < table_size) && (handlers_table[hstack] -> reserved == 1)){
        return 0;
    } else return 1;
}

unsigned int stack_size(const hstack_t hstack){   
    if (stack_valid_handler(hstack) == 0){
        if (handlers_table[hstack] -> p_top_node != NULL){
            unsigned int size = handlers_table[hstack] -> p_top_node -> node_number;
            return size;
        }
        else return 0;
    }
    return -1;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size) {     
    if ((stack_valid_handler(hstack) == 0)&&(data_in != NULL)){
        struct node* p_prev = handlers_table[hstack] -> p_top_node; //saving pointer to prev element
        int new_node_number = 1;
        if (p_prev != NULL) {
            new_node_number = p_prev -> node_number + 1; //increment number of node
        }
        handlers_table[hstack] -> p_top_node = (node_t*) malloc(sizeof(node_t));
        if (handlers_table[hstack] -> p_top_node == NULL) { //checking for correct memory allocation
            return;
        }
        handlers_table[hstack] -> p_top_node -> prev = p_prev;
        handlers_table[hstack] -> p_top_node -> node_number = new_node_number;
        handlers_table[hstack] -> p_top_node -> data = (char*) malloc (size);
        if (handlers_table[hstack] -> p_top_node -> data == NULL) { //checking for correct memory allocation
            return;
        }
        handlers_table[hstack] -> p_top_node -> size = size;
        char *data_out = handlers_table[hstack] -> p_top_node -> data;
        memcpy(data_out, data_in, size);
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if ((stack_valid_handler(hstack) == 0) //cheaking for correct input
        &&(data_out != NULL)
        &&(handlers_table[hstack] -> p_top_node != NULL)) {
        int size_to_copy = (handlers_table[hstack] -> p_top_node -> size > size) ? size : handlers_table[hstack] -> p_top_node -> size;
        char *data_in = handlers_table[hstack] -> p_top_node -> data;
        memcpy(data_out, data_in, size_to_copy);
        const struct node* prev_node = handlers_table[hstack] -> p_top_node -> prev;
        //Правильно ли освобождаю память?
        free((void*)handlers_table[hstack] -> p_top_node -> data); 
        free((void*)handlers_table[hstack] -> p_top_node);
        handlers_table[hstack] -> p_top_node = prev_node;
        return size_to_copy;
    }
}
#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node {
    struct node* prev;
    unsigned int size;
    char* data;
    unsigned int node_number;
} node_t;

typedef struct top_node_table {
    unsigned short int reserved;
    node_t* p_top_node;
} stack_t;

stack_t* handlers_table; 
int table_size = 0;

// Functions:

hstack_t stack_new(){
    hstack_t handler = -1;
    // search for empty space
    for (int i = 0; i < table_size; i++) {
        if (handlers_table[i].reserved == 0) {
            handlers_table[i].reserved = 1;
            handlers_table[i].p_top_node = NULL;
            return i;
        }
    }
    // array extension
    handlers_table = realloc(handlers_table, (table_size + 1) * sizeof(stack_t));
    if (handlers_table == NULL) { //checking for correct memory allocation
        return -1;
    }
    ++ table_size;
    handler = table_size - 1;
    handlers_table[handler].reserved = 1;
    handlers_table[handler].p_top_node = NULL;
    return handler;
}

void stack_free(const hstack_t hstack) {
    //checking for valid handler
    if (stack_valid_handler(hstack) == 1) {
        return;
    }
    node_t* top = handlers_table[hstack].p_top_node;
    //checking for 0 element
    if (top == NULL) {
        handlers_table[hstack].reserved = 0;
        return;
    }
    //delete all elements
    while (top != NULL) {
        node_t* prev_node = top -> prev;
        free(top -> data); 
        free(top);
        top = prev_node;
    }
    handlers_table[hstack].reserved = 0;
    //resize table if able
    unsigned int size_to_reduce = 0; //count how much we can reduce
    for (int i = table_size - 1; ((i >= 0)||(handlers_table[i].reserved == 0)); i--){
        ++ size_to_reduce;
    }
    //decrease
    if (size_to_reduce) {
        handlers_table = realloc(handlers_table, (table_size - size_to_reduce) * sizeof(stack_t));
    }
    --table_size;
}   

int stack_valid_handler(const hstack_t hstack){
    return !((hstack >= 0) && (hstack < table_size) && (handlers_table[hstack].reserved == 1));
}

unsigned int stack_size(const hstack_t hstack){   
    if (stack_valid_handler(hstack) == 0){
        node_t* top = handlers_table[hstack].p_top_node;
        if (top != NULL){
            unsigned int size = top -> node_number;
            return size;
        }
        return 0;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size) {     
    if ((stack_valid_handler(hstack) == 0)&&(data_in != NULL)&&(size >0)){
        node_t* top = handlers_table[hstack].p_top_node;
        node_t* p_prev = top; //saving pointer to prev element
        int new_node_number = 1;
        if (p_prev != NULL) {
            new_node_number = p_prev -> node_number + 1; //increment number of node
        }
        handlers_table[hstack].p_top_node = (node_t*) malloc(sizeof(node_t));
        if (handlers_table[hstack].p_top_node == NULL) { //checking for correct memory allocation
            return;
        }
        top = handlers_table[hstack].p_top_node;
        top -> prev = p_prev;
        top -> node_number = new_node_number;
        top -> data = (char*) malloc (size);
        if (top -> data == NULL) { //checking for correct memory allocation
            return;
        }
        top -> size = size;
        char *data_out = top -> data;
        memcpy(data_out, data_in, size);
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    node_t* top = handlers_table[hstack].p_top_node;
    if ((stack_valid_handler(hstack) == 0) //cheaking for correct input
        &&(data_out != NULL)
        &&(top != NULL)
        &&(size > 0)) {

        if (size >= top -> size) {
            char *data_in = top -> data;
            memcpy(data_out, data_in, top -> size);
            unsigned int result = top -> size;
            struct node* prev_node = top -> prev;
            free((void*)top -> data); 
            free((void*)top);
            handlers_table[hstack].p_top_node = prev_node;
            return result;
        }
        return 0;
    } 
    return 0;
}

int main()
{   
    while(1){
        char input;
        printf("\nChoose function:\nn - stack_new\nf - stack free\nv - stack_valid_handler\ns - stack_size\nu - stack_push\no - stack_pop\nq - quit\nYour char: ");
        scanf("%s", &input);

        if (input == 'n'){
            printf("%d\n", stack_new());
        }
        if (input == 'v'){
            printf("Enter the handler: ");
            int h;
            scanf("%d", &h);
            if (stack_valid_handler(h) == 0) {
                printf("Handler is valid\n");
            } else printf("Handler is NOT valid\n");
        }
        if (input == 'u'){
            printf("Enter the handler: ");
            int h;
            scanf("%d", &h);
            int x;
            printf("Enter the int to push: ");
            scanf("%d", &x);
            stack_push(h, &x, sizeof(x));
        }
        if (input == 'o') {
            printf("Enter the handler: ");
            int h;
            scanf("%d", &h);
            int y;
            stack_pop(h, &y, 4);
            printf("Popped: %d\n", y);
        }
        if (input == 'f'){
            printf("Enter the handler: ");
            int h;
            scanf("%d", &h);
            stack_free(h);
        }
        if (input == 's'){
            printf("Enter the handler: ");
            int h;
            scanf("%d", &h);
            printf("%d\n", stack_size(h));
        }

        if (input == 'q') {
            return 0;
        }
    }
    return 0;
}
#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define freed_stack_size -1
#define start_stack_i -1
#define error_code_stack_new -1
#define stack_valid_code 0
#define stack_invalid_code 1


typedef struct node
{
    struct node* prev;
    unsigned int size;
    char data[];
} node_t;


typedef struct stack
{
    struct node* top;
    int stack_size;
} stack_t;


typedef struct stack_table
{
    int reserved_size;
    int last_stack_i;
    struct stack* table;
} stack_table_t;


stack_table_t g_table = { .reserved_size = 16, .last_stack_i = start_stack_i, .table = NULL };



hstack_t stack_new()
{
    //check if this is the first stack in g_table
    if(g_table.last_stack_i == start_stack_i)
    {
        g_table.table = (stack_t*)malloc(g_table.reserved_size * sizeof(stack_t));


        //check NULL pointer
        if(g_table.table == NULL)
        {
            printf("Something went wrong at stage of creating stack_table_t\n");
            return error_code_stack_new;
        }
        else
        {
            printf("Stack table allocation went ok!\n");
        }
    }
    //check if this stack exceeds reserved number of stacks
    else if(g_table.last_stack_i == (g_table.reserved_size - 1))
    {
        //check for freed stack handlers that could be used to create new ones
        for (int i = 0; i <= g_table.last_stack_i; i++){
            //search for freed stack
            if(g_table.table[i].stack_size == freed_stack_size)
            {
                g_table.table[i].stack_size = 0;
                return i;
            }
        }

        
        //double the reserved size in g_table
        g_table.reserved_size = g_table.reserved_size * 2;
        g_table.table = (stack_t*)realloc(g_table.table, g_table.reserved_size * sizeof(stack_t));


        //check NULL pointer
        if(g_table.table == NULL)
        {
            printf("Something went wrong at stage of extending stack_table_t\n");
            return error_code_stack_new;
        }
        else
        {
            printf("Stack table reallocation went ok!\n");
        }
    }
    //adding new stack to stack_size counter
    g_table.last_stack_i++;
    //creating new stack
    g_table.table[g_table.last_stack_i] = (stack_t){ .top = NULL, .stack_size = 0};
    return g_table.last_stack_i;
}

void stack_free(const hstack_t hstack)
{   
    if(stack_valid_handler(hstack) == stack_valid_code)
    {
        node_t* current_top = g_table.table[hstack].top;
        if(current_top != NULL)
        {
            node_t* current_prev = current_top->prev;
            //successively delete stack nodes
            for(int i = 0; i < g_table.table[hstack].stack_size;i++)
            {
                free(current_top);
                current_top = current_prev;
                if(current_top != NULL) current_prev = current_top->prev;
            }
        }
        //making stack_invalid
        g_table.table[hstack].stack_size = freed_stack_size;
    }
}


int stack_valid_handler(const hstack_t hstack)
{
   return (
    ((hstack >= 0) 
    && (hstack <= g_table.last_stack_i)
    && (g_table.table[hstack].stack_size != freed_stack_size))
    ? stack_valid_code : stack_invalid_code);
}

unsigned int stack_size(const hstack_t hstack)
{
    return ((stack_valid_handler(hstack) == stack_valid_code)
     ? g_table.table[hstack].stack_size : 0);
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{   
    if((stack_valid_handler(hstack) == stack_valid_code) 
        && (data_in != NULL) && (size > 0))
    {
        //create new node
        node_t* new_node = (node_t*)malloc(sizeof(node_t) + sizeof(char) * size);


        //check NULL pointer
        if(new_node == NULL)
        {
            printf("Couldn't push new value because of memory error\n");
            return;
        }
        else
        {
            printf("Pushed new value successfully!\n");
        }
        //fill variables of new node
        new_node->size = size;
        new_node->prev = g_table.table[hstack].top;
        memcpy(new_node->data, data_in, size);
        //change the stack variables
        g_table.table[hstack].top = new_node;
        g_table.table[hstack].stack_size++;
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if((stack_valid_handler(hstack) == stack_valid_code)
        && (g_table.table[hstack].stack_size > 0)
        && (size >= g_table.table[hstack].top->size)
        && (data_out != NULL))
    {
        node_t* top = g_table.table[hstack].top;
        node_t* prev = top->prev;
        //change the data inside the stack
        g_table.table[hstack].top = prev;
        g_table.table[hstack].stack_size--;
        //copy data in data_out
        unsigned int size_to_transfer = top->size; 
        memcpy(data_out, top->data, size_to_transfer);
        //delete all data from temp_top
        free(top);
        return size_to_transfer;
    }
    return 0;
}




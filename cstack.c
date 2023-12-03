#include "cstack.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>


struct stack_head head = {0, NULL, 0, NULL};



hstack_t stack_new(void)
{
    //variable for new stack-descriptors
    static hstack_t current_id = 0;

    //Allocate mem for the new stack head
    struct stack_head* temp = head.p_prev;
    head.p_prev = malloc(sizeof(struct stack_head));

    //Initialize all fields of the new stack head
    head.p_prev->id = ++current_id;
    head.p_prev->p_prev = temp;
    head.p_prev->stack_size = 0;
    head.p_prev->p_node = NULL;
    
    return current_id;
}


void stack_free(const hstack_t hstack)
{
    struct stack_head* temp = &head;
    struct stack_head* head_before_temp = NULL;
    unsigned char found = 0u;

    // Finding needed stack
    while (temp->p_prev != NULL) {
        head_before_temp = temp;
        temp = temp->p_prev;

        if (temp->id == hstack) {
            found = 1u;
            break;
        }
    }

    // If the stack hasn't been found, do nothing
    if (found == 0u)
        return;

    // If the stack is empty, do nothing
    if (temp->stack_size == 0u)
        return;

    // Delete the relation between needed stack and stack that had a pointer to this stack
    head_before_temp->p_prev = temp->p_prev;


    //Delete all nodes that were in this stack
    struct node* temp_node = temp->p_node;
    struct node* prev_temp_node = temp_node->prev;
    free(temp);

    while (prev_temp_node != NULL) {
        free(temp_node->p_data);
        free(temp_node);
        temp_node = prev_temp_node;
        prev_temp_node = temp_node->prev;

    }

    //Delete the last  node
    free(temp_node->p_data);
    free(temp_node); 
}


int stack_valid_handler(const hstack_t hstack)
{
    struct stack_head* temp = &head;

    //Find needed stack and return 0 if it's found
    while (temp->p_prev != NULL) {
        temp = temp->p_prev;

        if (temp->id == hstack)
            return 0;
    }
    
    return 1;
}


unsigned int stack_size(const hstack_t hstack)
{
    struct stack_head* temp = &head;

    //Find needed stack and return its size
    while (temp->p_prev != NULL) {
        temp = temp->p_prev;

        if (temp->id == hstack)
            return temp->stack_size;
    }

    return 0u;
}


void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    struct stack_head* temp = &head;
    unsigned char found = 0u;

    // Finding needed stack
    while (temp->p_prev != NULL) {
        temp = temp->p_prev;

        if (temp->id == hstack) {
            found = 1u;
            break;
        }
    }

    // If the stack hasn't been found, do nothing
    if (found == 0u)
        return;

    // If the stack is found, allocate mem for new node
    // and set all fields of the new node
    ++(temp->stack_size);

    struct node* temp_node = temp->p_node;
    temp->p_node = malloc(sizeof(struct node));

    temp->p_node->prev = temp_node;
    temp->p_node->data_size = size;
    temp->p_node->p_data = malloc(sizeof(size));
    memcpy(temp->p_node->p_data, data_in, size);
}


unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    struct stack_head* temp = &head;
    unsigned char found = 0u;

    // Finding needed stack
    while (temp->p_prev != NULL) {
        temp = temp->p_prev;

        if (temp->id == hstack) {
            found = 1u;
            break;
        }
    }

    // If the stack hasn't been found, do nothing
    if (found == 0u)
        return 0u;

    // If the stack is empty, do nothing
    if (temp->stack_size == 0u)
        return 0u;


    // If the stack has at least one field
    // Delete and put data from the field to the buffer
    struct node* temp_node = temp->p_node;
    temp->p_node = temp_node->prev;
    --(temp->stack_size);

    //We set the buffer according to comparison of its size and size of the data
    if (temp_node->data_size <= size) {
        unsigned int copied_size = temp_node->data_size;

        memcpy(data_out, temp_node->p_data, copied_size);
        free(temp_node->p_data);
        free(temp_node);

        return copied_size;
    } else {
        memcpy(data_out, temp_node->p_data, size);
        free(temp_node->p_data);
        free(temp_node);
        return size;
    }
}


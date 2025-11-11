#include "cstack.h"
#include <stddef.h>

#define UNUSED(VAR) (void)(VAR)

hstack_t stack_new(void)
{
    struct stackB_t* newStack = malloc(sizeof(struct stackB_t));
    if(newStack == NULL) return -1;
    if(laststack != 0)
    {
        newStack->hstack = laststack->hstack + 1;
        newStack->prev = laststack;
    }
    else
    {
        newStack->prev = NULL;
        newStack->hstack = 1;
    }
    newStack->stackLast = NULL;
    laststack = newStack;
    return laststack->hstack;
}

void stack_free(const hstack_t hstack)
{
    if(laststack == NULL) return;
    struct stackB_t* stackDel = NULL;
    if(laststack->hstack == hstack)
    {
        stackDel = laststack;
        laststack = stackDel->prev;
    }
    else
    {
        if(laststack->prev == NULL) return;
        struct stackB_t* stackPrev = laststack;
        struct stackB_t* stackCurrent = laststack->prev;
        do
        {
            if(stackCurrent->hstack == hstack)
            {
                stackPrev->prev = stackCurrent->prev;
                stackDel = stackCurrent;
                break;
            }
            stackPrev = stackCurrent;
            stackCurrent = stackCurrent->prev;
        }
        while(stackCurrent != NULL);
    }
    if(stackDel == NULL) return;
    if(stackDel->stackLast != NULL) freeElem(stackDel->stackLast);
    free(stackDel);
}

void freeElem(struct stackElem_t* stackElem)
{
    if(stackElem->prev != NULL) freeElem(stackElem->prev);
    free(stackElem);
}

int stack_valid_handler(const hstack_t hstack)
{
    struct stackB_t* stackCurrent = stackB_search(hstack);
    return stackCurrent == NULL ? 1 : 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    struct stackB_t* stackCurrent = stackB_search(hstack);
    if(stackCurrent == NULL) return 0;
    struct stackElem_t* stackElemCurrent = stackCurrent->stackLast;
    unsigned int count = 0;
    while(stackElemCurrent != NULL)
    {
        stackElemCurrent = stackElemCurrent->prev;
        count++;
    }
    return count;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(data_in == NULL || size == 0) return;
    struct stackB_t* stackCurrent = stackB_search(hstack);
    if(stackCurrent == NULL) return;
    struct stackElem_t* newStackElem = malloc(sizeof(struct stackElem_t) + size);
    if(newStackElem == NULL) return;
    char* data = newStackElem->data;
    const char* data_in_p = data_in;
    while(data != newStackElem->data + size && data_in_p != data_in + size)
    {
        *data = *data_in_p;
        data++;
        data_in_p++;
    }
    newStackElem->size = size;
    newStackElem->prev = stackCurrent->stackLast == NULL ? NULL : stackCurrent->stackLast;
    stackCurrent->stackLast = newStackElem;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if(data_out == NULL || size == 0) return 0;
    struct stackB_t* stackCurrent = stackB_search(hstack);
    if(stackCurrent == NULL) return 0;
    unsigned int count = 0;
    struct stackElem_t* stackElemCurrent = stackCurrent->stackLast;
    if(stackElemCurrent == NULL) return 0;
    if(stackElemCurrent->size != size) return 0;
    char* data = stackElemCurrent->data;
    char* data_out_p = data_out;
    while(data != stackElemCurrent->data + size && data_out_p != data_out + size)
    {
        *data_out_p = *data;
        data++;
        data_out_p++;
        count++;
    }
    stackCurrent->stackLast = stackElemCurrent->prev;
    free(stackElemCurrent);
    return count;
}

struct stackB_t* stackB_search(const hstack_t hstack)
{
    if(laststack == NULL) return NULL;
    struct stackB_t* stackCurrent = laststack;
    while(stackCurrent != NULL)
    {
        if(stackCurrent->hstack == hstack) return stackCurrent;
        stackCurrent = stackCurrent->prev;
    }
    return NULL;
}


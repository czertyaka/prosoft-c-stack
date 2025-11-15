#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

struct Stack {
    struct StackElement* lastElPtr;
};

struct StackElement {
    struct StackElement* prevElement;
    void* data;
    unsigned int size;
};

struct StackElement* create_stack_element(void* data, unsigned int size, struct StackElement* prevElement) {

    // Проверка на адекватность входных данных
    if ((data == NULL) || (size == 0)) {
        return NULL;
    }
    struct StackElement* element = malloc(sizeof(struct StackElement));
    if (element == NULL) {
        return NULL;
    }

    element->data = malloc(size);

    memcpy(element->data, data, size);
    element->size = size;
    element->prevElement = prevElement;

    return element;
}

// Создаём stackArray
static int stackArrayCount = 0;
static struct Stack** stackArray = NULL;

hstack_t stack_new(void)
{
    // Добавление стека

    struct Stack* stack = malloc(sizeof(struct Stack)); // Выделяем память
    if (stack == NULL) return -1; // Не удалось создать стек
    stack->lastElPtr = NULL;  // Элементов в стеке пока что нет

    struct Stack** new_array = realloc(stackArray, (stackArrayCount + 1) * sizeof(struct Stack*));
    if (new_array == NULL) {
        // Не удалось создать список стеков
        free(stack);
        return -1;
    }
    stackArray = new_array;
    stackArray[stackArrayCount] = stack;

    int new_handle = stackArrayCount;
    stackArrayCount += 1;
    return new_handle;
}

void stack_free(const hstack_t hstack)
{
    // Удаление стека и его элементов

    if (stack_valid_handler(hstack) == 1) {
        // Если стека с таким hstack нет, то просто ничего не делаем
        return;
    }


    struct Stack** newStackArray = NULL;
    if (stackArrayCount > 1) {
        newStackArray = malloc((stackArrayCount - 1) * sizeof(struct Stack*));
    }

    int newIdx = 0;
    for (int i = 0; i < stackArrayCount; i++) {
        if (i == hstack) {
            // Нужно удалить все элементы рассматриваемого стека
            struct Stack* deletingStack = stackArray[i];
            struct StackElement* el = deletingStack->lastElPtr;
            while (el != NULL) {
                struct StackElement* nextEl = el->prevElement;
                free(el->data);
                free(el);
                el = nextEl;
            }
            free(deletingStack);
            continue;
        }
        // Переносим элементы старого списка в новый
        //cmake ругается, потому что не понимает, 
        // что сюда мы не попадём, если newStackArray == NULL,
        // равно как и обращения по несуществующему индексу
        newStackArray[newIdx] = stackArray[i];
        newIdx++;
    }
    // Высвобождаем память
    free(stackArray);
    stackArray = newStackArray;
    stackArrayCount -= 1;

}

int stack_valid_handler(const hstack_t hstack)
{
    // Проверка валидности заданного hstack

    // Все hstack являются последовательными числами от 0
    if ((stackArrayCount == 0) || (hstack < 0) || (hstack >= stackArrayCount)) {

        return 1;
    }
    else {
        return 0;
    }
}

unsigned int stack_size(const hstack_t hstack)
{
    // Количество элементов в стеке

    if (stack_valid_handler(hstack) == 1) {
        // Если такого стека нет, то и количество элементов = 0
        return 0;
    }

    int count = 0;
    struct Stack* currentStack = stackArray[hstack];
    struct StackElement* el = currentStack->lastElPtr;
    while (el != NULL) {
        count += 1;
        el = el->prevElement;
    }
    return count;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    // Добавление элемента в стек с заданным hstack

    if (stack_valid_handler(hstack) == 1) {
        // Если такого стека не существует, то ничего не делаем
        return;
    }
    struct Stack* currentStack = stackArray[hstack];
    struct StackElement* lastEl = currentStack->lastElPtr;
    struct StackElement* el = create_stack_element((void*)data_in, size, lastEl);
    if (el != NULL) {
        // Двигаем указатель на последний элемент, если он создался
        currentStack->lastElPtr = el;
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    // Получить последний элемент в стеке с заданным hstack
    if (stack_valid_handler(hstack) == 1) {
        // Если элемента нет, то размер записсанных данных = 0
        return 0;
    }

    struct Stack* currentStack = stackArray[hstack];
    struct StackElement* lastEl = currentStack->lastElPtr;

    // Если стек пустой
    if (lastEl == NULL) {
        return 0;
    }

    unsigned int sizeOfEl = 0;

    // Копируем данные в буфер, если он не NULLPTR
    if (data_out != NULL && size > 0) {
        sizeOfEl = (size < lastEl->size) ? size : lastEl->size;
        memcpy(data_out, lastEl->data, sizeOfEl);
    }
    else {
        // Если буфера нет, то элемент из стека все равно удалим и его размер передадим
        sizeOfEl = lastEl->size;
    }

    // Обнавляем указатель на последний элемент
    struct StackElement* preLastEl = lastEl->prevElement;
    currentStack->lastElPtr = preLastEl;

    // Освобождаем память
    free(lastEl->data);
    free(lastEl);

    return sizeOfEl;
}

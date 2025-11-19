#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>

#define UNUSED(VAR) (void)(VAR)

#define MAX_CAPACITY_STACK 10

/*  передаем пустое значение получаем int 
*   
*   если стек не созадн надо вренуть -1
*/

typedef struct                  // тип данных - элемент стека
{
    struct stackElementTypeDef* prev;   // Указатель на предыдущий элемент стека
    char* data;                         // Указатель на данные элемента стека (предполагаем что это char более подробно суть проблемы описана в строках 108)
    unsigned int size;                  // Размер данных элемента стека
    unsigned int temp;                  // Переменная не нужна, но без не екомпелятор ругается на выравниваение
    //int* data;
} stackElementTypeDef;

typedef struct                   // тип данных - контроль состояния стека
{
    unsigned int size[MAX_CAPACITY_STACK];  // количество данных в каждом из стеков
    unsigned int capacity;                  // общая емкость стека
    int numUsed;                            // количество используемых (инициализированных) стеков

  //  stackElementTypeDef massivHandPtr[MAX_CAPACITY_STACK];
    stackElementTypeDef** headPtr;          // Указатель на вершину стака (голову)
 //   stackElementTypeDef *head;
} stackListTypeDef;

stackListTypeDef stackList = { .numUsed = 0, .capacity = MAX_CAPACITY_STACK , .headPtr = NULL};




hstack_t stack_new(void)    
{
    if (stackList.numUsed < (MAX_CAPACITY_STACK - 2))   // Проверяем что количество используеых стеков меньше чем MAX_CAPACITY_STACK
    {
        if (stackList.numUsed == 0)                     // Проверка, что уже ранее использовался стек, иначе это превая попытка работы со стеком и надо обнулить переменные
        {
            for (char i = 0; i < MAX_CAPACITY_STACK; i++) 
            {
                stackList.size[i] = 0;
                stackList.headPtr[i] = NULL;
            }
        }


        stackList.numUsed++;

        stackList.headPtr[stackList.numUsed] = NULL;
        stackList.size[stackList.numUsed] = 0;

        return stackList.numUsed;

    }
    return -1;
}

void stack_free(const hstack_t hstack)
{
    unsigned int numStack = (unsigned int)hstack;   // номер стека записываем в numStack. имее право т.к. ранее проверено, что hstack не отрицаиеьный , без такого приведения компилятор ругается.

    if (stack_valid_handler(hstack))    // проверка валидности стека
    {

        while (!stackList.size[numStack])
        {
            stackElementTypeDef* steckDell = stackList.headPtr[numStack];   // Копируем адрес вершины стека, ранее првоерено что он не NULL
            stackList.headPtr[numStack] = steckDell->prev;                  // записываем адрес головы (верха) стека структуру контроля стека (будем удалять ее следующей)
           
            if (stackList.headPtr[numStack] != NULL)  // Доп. защита
            {
                free(steckDell->data);  // освобождаем место для данных в памяти
                free(steckDell);  // освобождаем место для стека в памяти
            }

            if (stackList.size[numStack] > 0)           stackList.size[numStack]--;  // Доп. защита
            else                                        stackList.size[numStack] = 0;

        }

        stackList.headPtr[numStack] = NULL;
        stackList.size[numStack] = 0;   // да повторяюсь но пусть будет.
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    if ((hstack < 0) || (hstack > (MAX_CAPACITY_STACK-1)))  return 1;       // Проверка, что номер стека в диапазоне от 0 до (MAX_CAPACITY_STACK-1), если номер в не допустимого диапазона возвразаем 1 
    if(stackList.headPtr[hstack] == NULL)                   return 1;           // Првоерка, что указатель на "голову стека" не 0
    
    return 0;  // валидация прошла успешно  
}

unsigned int stack_size(const hstack_t hstack)
{
    if(stack_valid_handler(hstack)) 
    {
        return stackList.size[hstack];
    }
    return 0;
}


void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    char* ptr;
    ptr = (char *)data_in;  // data_in типа const, следовательно компилятор не даст изменять его для прехода к следующей ячейке при копирвоании данных.
    /* почему char& - Тут проблема.Неизвестен тип передаваемых данных.скорее всего предаают указатель на структуру а в size передают размер структуры в байтах.
        В ТЗ нет указания какие данные передаются в стек.
        Поэтому делаю допущение, что тип данных char.
        Если передадут массив 2х байтных и более значений или только одно значение но размерностью не 1 байт, то скопируем не все и как следствие функция копирования стека работает не корректно 
        Если предположить что тип даных int то при передачи структуры или char скоприуется лишнее и потом при получении данных из стека вернется не все и не то.
    */

    
    if(size == 0)    return;
    if(data_in == NULL) return;
    if (stack_valid_handler(hstack)) 
    {   /* Валидация номера стека не прошла.
           Возможно не корректный номер или такого стека нет.
           Если тского стека нет, надо создать.
        */ 
        // првоеряем есть ли стек:
        if ((hstack < 0) || (hstack > (MAX_CAPACITY_STACK - 1))) return; // Получен некорректный номер стека, выходим из функции.
       
        // возможно такого стека еще нет, создаем его.
        if (hstack > stackList.numUsed)
        {
            stackList.numUsed++;    // увиличиваем кол-во контролируемых стеков

            stackList.headPtr[stackList.numUsed] = NULL;    // зануляем адрес первого элемента.
        }


    }

    unsigned int numStack = (unsigned int)hstack;   // номер стека записываем в numStack. имее право т.к. ранее проверено, что hstack не отрицаиеьный , без такого приведения компилятор ругается.


    // Выделение места в памяти для структуры с описанием нового элемента стека. В результате выделения памяти, адрес пространства памяти записывается в новую переменную. 
    stackElementTypeDef  *newStackElement = (stackElementTypeDef*)malloc(sizeof(stackElementTypeDef ));

    if (newStackElement == NULL)
    {   // место под данные не выделены, но ранее было выделено место для структуры и его надо освободить. 
        free(newStackElement);  // освобождаем место в памяти
        return; // Проверка корректности работы функции malloc. Результатом malloc может быть 0, если память не выделена.
    }




     newStackElement->data = (char*)malloc(size);

     if (newStackElement->data == NULL)    return; // Проверка корректности работы функции malloc. Результатом malloc может быть 0, если память не выделена.

     // Память выделена копируем данные.
     for (unsigned int i = 0; i < size; i++)
     {
         newStackElement->data[i] = *ptr;
         ptr++;
     }

     
     newStackElement->size = size;  // копирование размера элемента стека


     newStackElement->prev = stackList.headPtr[numStack];   // связывание нового элемента стека со старым
     stackList.headPtr[numStack] = newStackElement->prev;   // записываем адрес головы (верха) стека структуру контроля стека
     stackList.size[numStack] ++;                           // увеличение количество элементов стека в структуре контроля стека

    return;

}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    char* ptr;
    ptr = (char*)data_out;  // data_in типа const, следовательно компилятор не даст изменять его для прехода к следующей ячейке при копирвоании данных.
    /* 
    *   !!!! ранее оговорено, что тип данных стека (см. функцию добавления элемента в стек)
    */

    if(data_out == NULL) return 0;  // передан нулевой указатель
    if(size == 0) return 0;         // размер запрашиваемых байт 0 - ошибка
    if(stack_valid_handler(hstack)) return 0; // стек не валиден.

    unsigned int numStack = (unsigned int)hstack;   // номер стека записываем в numStack. имее право т.к. ранее проверено, что hstack не отрицаиеьный , без такого приведения компилятор ругается.
    
    stackElementTypeDef* steckRead = stackList.headPtr[numStack];   // Копируем адрес вершины стека, ранее првоерено что он не NULL

    if(steckRead->size != size)    return 0; //Проверка, что размер запрашиваемых данных совпал с размером элемента стека.

    // Копируем данные.
    for (unsigned int i = 0; i < size; i++)
    {
        *ptr = steckRead->data[i];
        ptr++;
    }

    /*
    *   Еще одна проблема. в ТЗ указано, что функция stack_pop должна "Brief: Извлечь элемент из стека и записать данные этого элемента в буфер, если соответствующий хэндлеру стек существует."
    *   В библиотеке есть функция stack_free() которая согласно ТЗ "Brief: Удалить стек, если соответствующий хэндлеру стек существует." удоляет весь стек.
    *   
    *   Но нигде явно не указано кто удаляет элемент стека. Предположу что после прочтения элемента стека его нужно удалить и перейти к предыдущему элементу стека.
    */


    if (stackList.size[numStack] > 0)   // првоерка, что не нулевой элемент стека
    {
        stackList.headPtr[numStack] = steckRead->prev;   // записываем в адрес головы (верха) стека укзатель на предыдущий элемент стека
        stackList.size[numStack]--;                      // уменьшаем количество элементов стека в структуре контроля стека
    }
    else
    {
        stackList.headPtr[numStack] = NULL;   
        stackList.size[numStack] = 0;                        
    }

    free(steckRead->data);  // освобождаем место для данных в памяти
    free(steckRead);  // освобождаем место для стека в памяти

    return 0;
}


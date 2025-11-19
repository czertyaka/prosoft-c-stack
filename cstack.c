#include "cstack.h"       // Подключаем заголовок
#include <stdlib.h>       
#include <string.h>       

#define UNUSED(VAR) (void)(VAR)   

// Узел стека, содержащий данные переменной длины
typedef struct node {
    struct node* prev;        // Указатель на предыдущий элемент стека
    unsigned int size;        // Размер данных
    char data[];              // Гибкий массив
} node_t;

// Элемент таблицы стеков
typedef struct stack_entry {
    int active;               // 1 — стек существует, 0 — ячейка свободна
    node_t* top;              // Указатель на вершину стека
    unsigned int count;       // Сколько элементов сейчас в стеке
} stack_entry_t;

// должно быть ≥10. .
#define STACK_TABLE_SIZE 16

// управляет всеми стеками библиотеки
static stack_entry_t g_table[STACK_TABLE_SIZE] = {0};


//хэндлер находится в диапазоне и активен
static int valid_handler(hstack_t h) {
    return (h >= 0 && h < STACK_TABLE_SIZE && g_table[h].active);
}

// Ищет первую свободную ячейку в таблице стеков
static int find_free_slot(void) {
    for (int i = 0; i < STACK_TABLE_SIZE; ++i) {
        if (!g_table[i].active)   // если стек не используется
            return i;             // возвращаем его индекс
    }
    return -1;   // свободных мест нет
}


// новый стек и  его х
hstack_t stack_new(void)
{
    int idx = find_free_slot();   // ищем свободную позицию для нового стека
    if (idx < 0)                  // если нет свободных мест
        return -1;                // ошибка

    g_table[idx].active = 1;      // отмечаем стек как активный
    g_table[idx].top = NULL;      // пока вершины нет
    g_table[idx].count = 0;       // элементов нет

    return idx;                   // возвращаем индекс
}


// Удаляет стек и очищает его содержимое
void stack_free(const hstack_t hstack)
{
    if (!valid_handler(hstack))   // проверяем 
        return;                   // если стек не существует

    node_t* n = g_table[hstack].top;   // начинаем с вершины

    // Удаляем все элементы стека 
    while (n) {
        node_t* prev = n->prev;   // сохраняем указатель на предыдущий
        free(n);                  // освобождаем текущий узел
        n = prev;                 
    }

    // Сбрасываем запись в таблице
    g_table[hstack].active = 0;
    g_table[hstack].top = NULL;
    g_table[hstack].count = 0;
}


// Проверяет
int stack_valid_handler(const hstack_t hstack)
{
    return valid_handler(hstack) ? 0 : 1;   // 0 — ок, 1 — ошибочный
}


// Возвращает количество элементов в стеке
unsigned int stack_size(const hstack_t hstack)
{
    if (!valid_handler(hstack))   // ошибка
        return 0;                 // по ТЗ вернуть 0

    return g_table[hstack].count; // иначе вернуть реальное количество
}


// Добавляет новый элемент в стек
void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (!valid_handler(hstack))   // стек существует
        return;
    if (!data_in || size == 0)    // данные норм
        return;

    //  сколько памяти нужно для узла:
    size_t total = sizeof(node_t) + size;

    node_t* n = malloc(total);    // выделяем память
    if (!n)
        return;                   //  мог не выделить

    n->prev = g_table[hstack].top;  // новый узел 
    n->size = size;                 // сохраняем размер данных

    memcpy(n->data, data_in, size); // копируем данные 

    g_table[hstack].top = n;        // вершина стека теперь — новый узел
    g_table[hstack].count++;        // увеличиваем счётчик элементов
}


// Извлекает верхний элемент
unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (!valid_handler(hstack))   // если стек не существует
        return 0;

    node_t* top = g_table[hstack].top;   // получаем указатель на вершину
    if (!top)                     // если пусто
        return 0;

    g_table[hstack].top = top->prev;   // смещаем вершину на предыдущую
    g_table[hstack].count--;           // уменьшаем размер

    // Сколько реально можем скопировать
    unsigned int nbytes = (size < top->size) ? size : top->size;

    // Если буфер есть и размер больше 0 — копируем
    if (data_out && nbytes > 0)
        memcpy(data_out, top->data, nbytes);

    free(top);   // освобождаем память узла

    return nbytes;   // возвращаем количество реально записанных байт
}


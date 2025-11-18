#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>   // malloc, realloc, free
#include <string.h>   // memcpy


// у нас динамический массив

// каждый элемент стек это узел node
struct node
{
    const struct node* prev; // указатель на предыдщий элемент стека
    unsigned int size; // размер данных, которые лежат в этом узле
    char data[0];
};
// stack_t это вершина стека
typedef struct node* stack_t; // указатель на стек

// завел глобальный динамический массив
struct stack_entry
{
    int reserved;                  // 1 = ячейка занята, 0 = свободна
    stack_t stack;                 // Указатель на вершину стека
};

// Удобный псевдоним для структуры stack_entry
typedef struct stack_entry stack_entry_t;

// Таблица стеков — динамический массив записей.
struct stack_entries_table
{
    unsigned int size;  // Сколько элементов сейчас в таблице
    stack_entry_t* entries;  // Указатель на динамический массив записей
};

// Глобальная таблица стеков.
// Изначально пустая: size=0, entries=NULL.
struct stack_entries_table g_table = { 0u, NULL };


hstack_t stack_new(void)
{
    // сначла ищем свободyную ячекй
    for (unsigned i = 0; i < g_table.size; i++)
    {
        if (g_table.entries[i].reserved == 0)
        {
            g_table.entries[i].reserved = 1; // помечаем запись как занятую
            g_table.entries[i].stack = NULL;  // новый стек изначально пуст
            return i;                // индекс записи — это хэндлер стека
        }
    }
    // теперь если мест нет нужно расширить таблицу
    unsigned new_size = g_table.size + 1;
    stack_entry_t* new_entries =
        realloc(g_table.entries, new_size * sizeof(stack_entry_t));

    if (!new_entries)
        return -1; // ошибка → вернуть -1 (как требует ТЗ)

    g_table.entries = new_entries;

    // инициализируем новую запись
    g_table.entries[g_table.size].reserved = 1;
    g_table.entries[g_table.size].stack = NULL;
    g_table.size = new_size;

    return g_table.size - 1;  // Возвращаем индекс нового стека
}

void stack_free(const hstack_t hstack)
{
    // Проверяем корректность хэндлера
    if (stack_valid_handler(hstack) == 1)
        return;

    // Получаем запись стека
    stack_entry_t* entry = &g_table.entries[hstack];

    // освобождаем узлы стека
    struct node* current = entry->stack;
    while (current != NULL)
    {
        struct node* prev = (struct node*)current->prev;  // prev — const*, нужно привести

        free(current);  // освобождаем сам узел
        current = prev; // двигаемся дальше
    }

    entry->stack = NULL;
    entry->reserved = 0;

}

// Проверить хэндлер, по тз 0 - соответствующий хэндлеру стек существует, 1 - нет.
int stack_valid_handler(const hstack_t hstack)
{
    // Нельзя использовать отрицательный индекс
    if (hstack < 0)
        return 1;
    // Нельзя выходить за пределы массива
    if ((unsigned)hstack >= g_table.size)
        return 1;
    // Проверяем, действительно ли стек существует
    if (g_table.entries[hstack].reserved == 0)
        return 1;
    // Всё корректно
    return 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    
    // Проверяем корректность хэндлера
    if (stack_valid_handler(hstack) == 1)
        return 0;

    // Получаем запись стека
    stack_entry_t* entry = &g_table.entries[hstack];

    // Идём по цепочке узлов и считаем количество элементов
    unsigned int count = 0;
    struct node* current = entry->stack;

    // идем от вершины к NULL
    while (current != NULL)
    {
        count++;
        current = (struct node*)current->prev;   // prev — const*, приводим обратно
    }

    return count;

}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    // проверка валиднсти
    if (stack_valid_handler(hstack) == 1)
        return;
    // проверка входных данных
    if (data_in == NULL || size == 0)
        return;
    // получаем запись стека, там хранится вершина стека
    stack_entry_t* entry = &g_table.entries[hstack];
    // выделяем память под новый узел стека
    struct node* new_node = malloc(sizeof(struct node) + size);
    // Если malloc вернул NULL — ошибка, ничего не делаем
    if (!new_node)
        return;

    new_node->prev = entry->stack; // new_node указывает на стару вершину
    new_node->size = size; // сохраняем сколько байт данных лежит в этом узле стека
    // побайтовое копирование 
    memcpy(new_node->data, data_in, size);

    // делаем новый узел вершины
    entry->stack = new_node;

}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    // проверяем хэндлер
    if (stack_valid_handler(hstack) == 1)
        return 0;
    // проверка входных данных
    if (data_out == NULL || size == 0)
        return 0;
    // получаем запись стека, там хранится вершина стека
    stack_entry_t* slot = &g_table.entries[hstack];
    // берем текущий узел, то есть вершину стека, если ее нет - стек пуст
    struct node* top_node = slot->stack;
    if (top_node == NULL)
        return 0;

    if (size != top_node->size)
        return 0;

    // Копируем данные из узла в буфер
    memcpy(data_out, top_node->data, size);

    slot->stack = (struct node*)top_node->prev;

    free(top_node);

    return size;

}


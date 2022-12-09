#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define RESERVED 1
#define NOT_RESERVED 0

//глобальная переменная для подсчета всех выделенных хендлеров
//на случай, если было выделено I хендлеров, но обслуживается
//i<<I стеков (к примеру, было создано 100 стеков, 95 конечных
//было освобождено, обслуживается 5 первых (g_table.size == 5),
//хотя реаллоцировано памяти для g_table.entries все еще на все 100)
static unsigned int handler_max;

struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
};

struct stack_entry
{
    int reserved;
    //stack_t stack;        //хендлер текущего стека? (0, 1, 2...i)
    //должен быть указатель на top-узел stack_entries_table.entries[i-го] стека
    struct node* top;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};    
    
hstack_t stack_new(void)
{
    hstack_t handler;
    int reserved_exist = 0;
    
    //если в массиве ничего нет, выделяем место под 1-й стек с индексом 0
    if (g_table.entries == NULL)
    {
        g_table.entries = (stack_entry_t*)calloc(1, sizeof(stack_entry_t));
        if (g_table.entries == NULL)
            return -1;
        handler_max = 0; //наибольшее значение у хендлеров (индекс) при создании первого стека
    }

    //проверка на наличие незарезервированных хендлеров; на 1 цикле
    //1-й стек еще не зарезервирован, здесь ему присваивается хендлер 0
    for (int i = 0; i <= (int)handler_max; ++i)
    {
        if (!g_table.entries[i].reserved)
        {
            handler = i;
            reserved_exist = 1;
            break;
        }
    }

    //для остальных новых стеков реаллоцируем память
    if (!reserved_exist)
    {
        handler_max += 1u;
        handler = (int)handler_max;
        g_table.entries = (stack_entry_t*)realloc(g_table.entries, (handler_max+1) * sizeof(stack_entry_t));
        if (g_table.entries == NULL)
            return -1;
    }

    g_table.size += 1u;
    g_table.entries[handler].reserved = RESERVED; //здесь на 1 цикле 1 стек с 0 хендлером резервируется
    g_table.entries[handler].top = NULL;
    return handler;
}

int stack_valid_handler(const hstack_t hstack)
{
    if (hstack <0 || hstack > (int)handler_max || g_table.size == 0u)
        return 1;

    if (g_table.entries[hstack].reserved == RESERVED)
        return 0;
    else return 1; //на всякий пожарный
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
        return;
    
    struct node *temp = NULL;
    struct node* current_top = g_table.entries[hstack].top;
    while (current_top != NULL)
    {
        temp = current_top;
        current_top = (struct node *)current_top->prev;
        free(temp);
        temp = NULL;
    }
    g_table.entries[hstack].reserved = NOT_RESERVED;
    g_table.size -= 1u;

    //находим хендлер, на котором кол-во зарезервированных стеков count
    //становится равным размеру g_table.size, если g_table.size и handler_max не совпадают
    if (g_table.size <= handler_max)
    {
        unsigned int handler;
        unsigned int count = 0u;
        for (handler = 0u; handler <= handler_max; ++handler)
        {
            if (g_table.entries[handler].reserved == 1)
            {
                ++count;
                if (count == g_table.size)
                break;
            }
        };
        //если найденный handler меньше обзего кол-ва хендлеров (начиная с
        //handler+1 стека идут только пустые незарезервированные стеки), то
        //реаллоцируем память под handler+1 (с учетом нулевого) обслуживаемых стеков
        if (handler < handler_max)
        {
            stack_entry_t *rewrite_entries = (stack_entry_t *)calloc((handler+1u), sizeof(stack_entry_t));
            if (rewrite_entries == NULL)
                    return;
            for (unsigned int i = 0; i <= handler; ++i)
                rewrite_entries[i] = g_table.entries[i];
                        
            g_table.entries = (stack_entry_t*)realloc(g_table.entries, (handler + 1u) * sizeof(stack_entry_t));
            if (g_table.entries == NULL)
                return;
            for (unsigned int i = 0; i <= handler; ++i)
                g_table.entries[i] = rewrite_entries[i];
            free(rewrite_entries);
            rewrite_entries = NULL;
            handler_max = handler;
        }
    }

    if (g_table.size == 0u)
    {
        free(g_table.entries);
        g_table.entries = NULL;
        return;
    }
    return;
}


unsigned int stack_size(const hstack_t hstack)
{
    unsigned int count = 0u;
    
    if (stack_valid_handler(hstack))
        return count;
    
    const struct node *tmp = g_table.entries[hstack].top;
    while (tmp != NULL)
    {
        tmp = tmp->prev;
        count += 1u;
    }
    return count;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if (stack_valid_handler(hstack) || data_in == NULL || size <= 0)
        return;
    
    //создание node для нового top-узла с проверкой на проблемы с памятью
    struct node *new_top = (struct node *)malloc(sizeof(struct node) + size);
    if (new_top == NULL)
        return;
    
    //создаем указатель на указатель data_in, приведенный к char *, для копирования
    const char *src = (char *)data_in;
    for (unsigned int i = 0; i < size; ++i)
        new_top->data[i] = src[i];
    new_top->size = size;
    new_top->prev = g_table.entries[hstack].top;
    g_table.entries[hstack].top = new_top;
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    unsigned int size_of_data = 0u;
    if (stack_valid_handler(hstack) || data_out == NULL || size <= 0 || g_table.entries[hstack].top == NULL)
        return size_of_data;
    
    //создаем указатель на указатель data_out для посимвольного копирования
    char* output = (char*)data_out;
    struct node* temp = g_table.entries[hstack].top;

    if (g_table.entries[hstack].top->size <= size)
    {
        for (int i = 0; i < (int)temp->size; ++i)
            output[i] = temp->data[i];
        size_of_data = size;
    }
    //если размер буфера меньше размера хранимых данных в top-узле,
    //произойдет потеря данных из-за недостаточного размера size
    else return 0u;
    
    g_table.entries[hstack].top = (struct node *)g_table.entries[hstack].top->prev;
    free(temp);
    temp = NULL;
    return size_of_data;
}


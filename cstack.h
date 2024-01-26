#ifndef CSTACK_H
#define CSTACK_H
#pragma warning(disable: 4820)
#pragma warning(disable: 4018)
#pragma warning(disable: 4312)

struct node // структура, описывающая конкретный узел данного стека
{
    void* data_void;   // поле данных
    struct node* prev; // указатель на предыдущий элемент
    unsigned int size; // размер данных
};
//typedef struct node* node_t;

struct stack_entry // структура, описывающая конкретный стек
{
    int reserved; // показывает число элементов в стеке (если -1, то стек не создан)
    struct node* stack; // верхушка стека - указатель на верхний узел
};

struct stack_entries_table // структура, представляющая собой таблицу стеков
{
    int size;      // количество стеков
    struct stack_entry* entries[20]; // указатель на конкретный стек
};

typedef int hstack_t;
//struct node;
//struct node_t;
//typedef struct node* node_t;
//struct stack_entry;
//typedef struct stack_entry stack_entry_t;
//struct stack_entries_table;

hstack_t stack_new(void);
void stack_free(const hstack_t stack);
int stack_valid_handler(const hstack_t stack);
unsigned int stack_size(const hstack_t stack);
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */

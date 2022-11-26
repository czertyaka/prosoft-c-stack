
#include "stdio.h"
#include <assert.h>
#include "cstack.c"
#include "cstack.h"

int main(int argc, const char * argv[]) 
{
    /*int a = 10;
    int b = 11;
    const hstack_t stack = stack_new();
    printf("%d ", stack);
    char str[2] = "AB";
     
    char* ptr = str;
    const void* st = ptr;
    stack_push(stack, st, sizeof(str));
    printf("size %d ", stack_size(stack));
    char* dataout = malloc(10);
   // stack_pop(stack, st, dataout, sizeof(dataout));
    printf("size %d ", stack_size(stack));*/
    //23.11 занятие
    /*
    тип void
    квалификатор типа - const, volatile - указывает компилятору что переменная может меняться в др потоке и пр., restrict 
    */
    //int a = 0;
    //short b = 10;
    //printf("JJ");
    //char A = 'A';
    //printf("%c ", A);// print char
    //printf("%d ", A);
    //assert(sizeof(a) == sizeof(b));
    // размер переменной указателя соотв размеру машинного слова sizeof(size_t)
    /*int d = 17;
    int* p_d = &d;
    assert(sizeof(p_d) == sizeof(size_t));
    int** pp_d = &p_d;
    int* ppp_d = p_d;
    printf("%d", sizeof(size_t));*/
    // array
    int im[5];
    assert(sizeof(im) == sizeof(int)*5);
    // i++ ++i
    // сборка проекта
    /*
    папка сурс и папка билд
    сгенерировать файл сборки командой cmake
    cmake /src скачать 
    появится makefile
    make -j8
    запускаем cstack_test
    */

   
    
    /*int stack = stack_new();
    assert(stack_size(stack) == 0u);
    int data_out[3] = {1, 1, 1};
    int data_in[3] = {0, 1, 2};
    stack_push(stack, data_in, sizeof(data_in));
    assert(stack_pop(stack, data_out, sizeof(data_out)) == sizeof(data_out));
    assert(data_out[2] == 2 && data_out[1] == 1);
    stack_free(stack);
    assert(stack_size(stack) == 0u); 
    assert(stack_valid_handler(stack) == 1);

    int a = 12;
    int* ptr = malloc(sizeof(int*));
    ptr = NULL;*/
  
   /* const size_t count = 10;
    hstack_t stacks[10] = {-1};
    for (size_t i = 0; i < count; ++i)
    {
        stacks[i] = stack_new();
        assert(stack_valid_handler(stacks[i]) == 0);
        assert(stack_size(stacks[i]) == 0u);
    }
    for (size_t i = 0; i < count; ++i)
    {
        stack_free(stacks[i]);
        assert(stack_valid_handler(stacks[i]) == 1);
    }
    */
    //assert(stack_pop(stack, data_out, sizeof(data_out)) == sizeof(data_out));
   // assert(stack_size(stack) == 0u);

    /*hstack_t stack = stack_new();
    const size_t size = 3;
    const int data_in[3] = {0, 1, 2};
    int data_out[3] = {0, 1, 2};
    for (size_t i = 0; i < size; ++i)
    {
        stack_push(stack, &data_in[i], sizeof(data_in[i]));
        assert(stack_size(stack) == i + 1u);
    }
    for (size_t i = 0; i < size; ++i)
    {
        assert(stack_pop(stack, &data_out[i], sizeof(data_out[i])) == sizeof(data_out[i]));
        assert(stack_size(stack) == size - 1u - i);
    }
    assert(data_out[0] == 2 && data_out[1] == 1 && data_out[2] == 0);*/
   
    /*const size_t count = 10;
    hstack_t stacks[10] = {-1};
    for (size_t i = 0; i < count; ++i)
    {
        stacks[i] = stack_new();
        assert(stack_valid_handler(stacks[i]) == 0);
        assert(stack_size(stacks[i]) == 0u);
    }
    for (size_t i = 0; i < count; ++i)
    {
        stack_free(stacks[i]);
        assert(stack_valid_handler(stacks[i]) == 1);
    }

*/ hstack_t stack = stack_new();
    const size_t size = 5;
    const int data_in[5] = {1};
    stack_push(stack, &data_in, sizeof(data_in));
    assert(stack_size(stack) == 1u);

    assert(stack_pop(stack, NULL, 0u) == 0u);

    int data_out[5 - 1] = {0};
    assert(stack_pop(stack, data_out, sizeof(data_out)) == 0u);
    assert(data_out[1] == 0);

    assert(stack_pop(stack, NULL, sizeof(data_in)) == 0u);
    assert(data_out[1] == 0);
   /* hstack_t stack = stack_new();
    const int data_in[3] = {0, 1, 2};
    int data_out[3] = {2, 1, 0};
    stack_push(stack, data_in, sizeof(data_in));
    assert(stack_size(stack) == 1u);
    assert(stack_pop(stack, data_out, sizeof(data_out)) == sizeof(data_out));
    assert(stack_size(stack) == 0u);
    assert(data_out[0] == 0 && data_out[1] == 1 && data_out[2] == 2);
*/
    return 0;
}


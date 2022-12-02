#include"cstack.h"

int main()
{
   char hell[6] = "Hello";
   hstack_t Stak1 = stack_new();
   stack_push(Stak1,hell, sizeof(hell));

}
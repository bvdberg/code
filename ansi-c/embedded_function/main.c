#include <stdio.h>


typedef int (*funptr_t)(void);

funptr_t fun2(void) {
	int k = 0;

   int fun(void)
   {  return ++k;
   }

   printf("%d\n", fun());
   printf("%d\n", fun());
   printf("%d\n", fun());

   return fun;
}

int main(void) {
	funptr_t ptr;

   ptr = fun2();
   printf("%d\n", ptr());
   printf("%d\n", ptr());

   return 0;
}


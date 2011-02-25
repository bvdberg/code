#include <stdio.h>

typedef int (*func_ptr)(int a, int b);

void run_function(func_ptr func) {
    func(2, 5);
}

void select_function(int num) {
    int function_a(int a , int b)
    {
        printf("%s() \n", __func__);
        return a+b;
    }
    int function_b(int a , int b)
    {
        printf("%s() \n", __func__);
        return a*b;
    }
    func_ptr func = function_a;
    if (num > 5) func = function_b;
    run_function(func);
}


int main()
{
    select_function(10);
    select_function(2);

    void test(int a) {
        printf("%s() a=%d\n", __func__, a);
    }
    test(10);

    return 0;
}


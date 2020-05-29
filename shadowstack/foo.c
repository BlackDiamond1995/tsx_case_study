#include <stdio.h>

//extern unsigned long long k;

int foo1(int i)
{
    return i;
}

int foo(int i)
{
    return foo1(i);
}

int main()
{
    // unsigned long long b = &k;
    int a = 0;
    a = foo(2);
    printf("%d\n", a);
    return 0;
}
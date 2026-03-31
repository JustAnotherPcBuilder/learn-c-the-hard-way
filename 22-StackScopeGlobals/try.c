#include <stdio.h>

int add(int x)
{
    int value = 2;
    value += x;
    return value;
}

int static_add(int x)
{
    static int value = 2;
    value += x;
    return value;
}

int main()
{
    int x = 0;
    int y = 0;
    for(int i = 0; i < 5; i++)
    {
        x = add(x);
        y = static_add(y);
        printf("Iteration %d\n", i+1);
        printf("Without static: %d\n", x);
        printf("With static: %d\n\n", y);
    }
    return 0;
}
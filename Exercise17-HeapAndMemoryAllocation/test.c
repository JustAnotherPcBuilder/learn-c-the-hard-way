#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
int main()
{
    const char str[] = "123abc";
    printf("str: %s\n", str);
    char *str2 = "This is a string";
    char **ptr = &str2;
    printf("ptr: %p\n", ptr);
    printf("*ptr: %s\n", *ptr);
    printf("strtol: %ld\n", strtol(str, ptr, 10));
    printf("ptr: %p\n", ptr);
    printf("*ptr: %s", *ptr);

    return EXIT_SUCCESS;
}

/*
str: 123abc
ptr: 0x7ffdf4013980
*ptr: This is a string
strtol: 123
ptr: 0x7ffdf4013980
*ptr: abc
*/
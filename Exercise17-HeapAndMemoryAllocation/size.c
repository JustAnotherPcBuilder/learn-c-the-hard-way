#include <stdio.h>
#define MAX_DATA 512
struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};
int main(){
    printf("Int size: %zu\n", sizeof(int) );
    printf("Char pointer size %zu\n", sizeof(char *));
    printf("Struct Address size %zu\n", sizeof(struct Address));
}
// In this part of the exercise, I want you to attempt something difficult for the extra credit: Convert
// this program to not use pointers and malloc. This will be hard, so you’ll want to research the
// following:
// • How to create a struct on the stack, just like you’re making any other variable.
// • How to initialize it using the x.y (period) character instead of the x->y syntax.
// • How to pass a structure to other functions without using a pointer.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

struct Person Person_create(char *name, int age, int height,
        int weight)
{
    struct Person who = { name, age, height, weight };
    return who;
}


void Person_print(struct Person *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[])
{
    // make two people structures
    struct Person joe = Person_create("Joe Alex", 32, 64, 140);

    struct Person frank = Person_create("Frank Blank", 20, 72, 180);

    // print them out and where they are in memory
    printf("Joe is at memory location %p:\n", &joe);
    Person_print(&joe);

    printf("Frank is at memory location %p:\n", &frank);
    Person_print(&frank);

    // make everyone age 20 years and print them again
    joe.age += 20;
    joe.height -= 2;
    joe.weight += 40;
    Person_print(&joe);

    frank.age += 20;
    frank.weight += 20;
    
    Person_print(&frank);


    return 0;
}

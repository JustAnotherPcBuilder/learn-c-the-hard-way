// • Use scanf to write a function that reads one character at a time and fills in the names
// but doesn’t go past the end. Make this function generic so it can take a size for the string,
// but just make sure you end the string with '\0' no matter what.

#include <stdio.h>
#include "dbg.h"
#include <string.h>

#define MAX_DATA 100

typedef enum EyeColor {
    BLUE_EYES, GREEN_EYES, BROWN_EYES,
    BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
    "Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
    int age;
    char first_name[MAX_DATA];
    char last_name[MAX_DATA];
    EyeColor eyes;
    float income;
} Person;

char *read_1_char_at_a_time(char *buffer, int size){
    char ch;
    int len = 0;
    // guarantees all elements are null char; no need to set the end to null
    memset(buffer, '\0', size); 

    // Keep reading until either end of file or buffer is reached
    while ( scanf("%c", &ch) == 1 && len < size - 1 && ch != '\n'){
        buffer[len] = ch;
        len++;
    }
    if (len > 0) return buffer;
    return NULL;
}

int main (int argc, char *argv[])
{
    Person you = {.age = 0};
    int i = 0;
    char *in = NULL;

    printf("What's your First Name? ");
    // in = fgets(you.first_name, MAX_DATA - 1, stdin);
    in = read_1_char_at_a_time(you.first_name, MAX_DATA);
    check(in != NULL, "Failed to read first name.");

    printf("What's your Last Name? ");
    // in = fgets(you.last_name, MAX_DATA - 1, stdin);
    in = read_1_char_at_a_time(you.last_name, MAX_DATA);
    check(in != NULL, "Failed to read last name.");
    printf("How old are you? ");
    int rc= fscanf(stdin, "%d", &you.age);
    check(rc > 0, "You have to enter a number.");

    printf("What color are your eyes:\n");
    for (i = 0; i <= OTHER_EYES; i++){
        printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
    }
    printf("> ");
    int eyes = -1;

    rc = fscanf(stdin, "%d", &eyes);
    check(rc > 0, "You have to enter a number.");

    you.eyes = eyes - 1;
    check(you.eyes <= OTHER_EYES 
        && you.eyes >= 0, "Do it right, that's not an option.");
    
    printf("How much do you make an hour? ");
    rc = fscanf(stdin, "%f", &you.income);
    check(rc > 0, "Enter a floating point numnber.");

    printf("----- RESULTS -----\n");

    printf("First Name: %s\n", you.first_name);
    printf("Last Name: %s\n", you.last_name);
    printf("Age: %d\n", you.age);
    printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
    printf("Income: %f\n", you.income);

    return 0;

    error:

    return -1;
}
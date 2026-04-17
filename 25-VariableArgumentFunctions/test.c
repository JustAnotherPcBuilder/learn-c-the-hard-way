#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100
#define ASCII_OFFSET 48

int write_int(int out_int)
{
    if (out_int == 0){
        fputc('0', stdout);
        return 0;
    }

    int buffer[MAX_DATA];
    int i;
    for (i = 0; i < MAX_DATA; i++){
        buffer[i] = 0;
    }

    if(out_int < 0){
        fputc('-', stdout);
        out_int = -out_int;
    }

    // Use modulus to get remainder and find the digits from right to left
    for(i = 0; out_int > 0; i++){
        buffer[i] = out_int % 10;
        out_int -= buffer[i];
        out_int /= 10;
    }
    // Need to reverse the operation to print properly (left to right)
    for (i -= 1; i >=0; i--){
        fputc((char) (buffer[i] + ASCII_OFFSET), stdout);
    }

    return 0;
}

// int write_string(const char *out_string, int size)
// {
//     return 0;
// error:
//     return -1;
// }

// int write_print(const char *fmt, ...){
//     int i = 0;
//     int rc = 0;
//     int out_int = 0;
//     char out_char = NULL;
//     char *out_string = NULL;
//     int max_buffer = 0;

//     va_list argp;
//     va_start(argp, fmt);

//     for(i = 0; fmt[i] != '\0'; i++){
//         if (fmt[i] == '%'){
//             i++;
//             switch(fmt[i]){
//                 case '\0':
//                     sentinel("Invalid format, you ended with %%.");
//                     break;
//                 case 'd':
//                     out_int = va_arg(argp, int);
//                     rc = write_int(out_int);
//                     check(rc == 0, "Failed to write int.");
//                     break;
//                 case 'c':
//                     out_char = va_arg(argp, char);
//                     rc = fputc(out_char, stdout);
//                     check(rc == 0, "Failed to write char.");
//                     break;
//                 case 's':
//                     max_buffer = va_arg(argp, int);
//                     out_string = va_arg(argp, char *);
//                     rc = write_string(out_string, max_buffer);
//                     check(rc == 0, "Failed to write string.");
//                     break;
//                 default:
//                     sentinel("Invalid format.");
//             }  
//         }else {
//             fputc(fmt[i], stdout);
//         }
//         check(!ferror(stdout), "Output error.");
//         // check(!feof(stdin) && !ferror(stdin), "Input error.");
//         // check(!ferror(stdin), "Input error.");
//     }

//     va_end(argp);
//     return 0;
// error:
//     va_end(argp);
//     return -1;
// }

int main(int argc, char *argv[])
{
    int test[] = {0, -101, 34524, 12042, -93845, 93934};
    
    for (int i = 0; i < 6; i++){
        fputs("Test ", stdout);
        write_int(i);
        fputs(": ", stdout);
        write_int(test[i]);
        fputc('\n', stdout);
    }
    
    return 0;

}
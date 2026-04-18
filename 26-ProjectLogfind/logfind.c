/*
logfind
--------
usage: logfind pattern

checks all locations spedified in ~/.logfind and returns all matching patterns
1. multiple patterns separated by space are treated as 'and'
2. -o newPattern treats it as a logical 'or'
3. output list of files can be anything that glob function allows (see man 3 glob)
*/

#include <stdio.h>
#include <glob.h>


int main(int argc, char **argv)
{
    return 0;
}
/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // returns 1 : ends the program as terminal must have at least two arguments (including ./generate) and a maximum of 3 arguments
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // invokes atoi() function in stdlib.h which converts a string containing digits to its corresponding number to store in an integer variable
    int n = atoi(argv[1]);

    // if no. of arguments in terminal is 3, sends the second argument,[ converted to int by atoi() ], as seed, else sends the value returned from time(NULL) as seed. (Time having a NULL argument does not store the value and returns time since the epoch)
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // loops from 0 to n (the amount of numbers to be generated as asked by user) and invokes drand48 converting the long output to an int and printing it. each number is followed by a newline character
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}

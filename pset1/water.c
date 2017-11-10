#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Minutes:");  // prompts user for no. of minutes in the shower
    int m=get_int();
    printf("Bottles: %i\n", m*12); // goes to next line and prints equivalent amount of bottles of water
    return 0;
}
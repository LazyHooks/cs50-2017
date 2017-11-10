#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    
    for(int i=0, n=strlen(s); i<n ; i++)        // runs a loop from o to string length 
    {
        if(i==0)                                // checks if its the first character
        {
            if(s[i]!=' ')                       // if the first character is not a space , thus an alphabet, it is the first letter of the name and gets printed
            {
                printf("%c", toupper(s[i]));    // prints 1st initial in upper case
            }
        }
        else
        {
            if(s[i]!=' ' && s[i-1]==' ')        // checks if the character is not a space itself but has a space before it, so is the starting letter of a new word
            {
                printf("%c",toupper(s[i]));     // prints the letter as an Upper Case initial
            }
        }
    }
    printf("\n");
    
}
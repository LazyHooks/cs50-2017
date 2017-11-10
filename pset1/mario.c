#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height=0;
    printf("Height:");
    while(true)
    {
        height=get_int();
        if(height>=0 && height<=23)
        {
            break;
        }
        else
        {
            printf("retry:");
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<height-1-i;j++) //loop for spaces before #s
        {
            printf(" ");
        }
        
        for(int j=0;j<=i;j++)   //loop to print first series of #s
        {
            printf("#");
        }
        
        printf("  ");    //middle space
        
        for(int j=0;j<=i;j++)
        {
            printf("#");
        }
        
        printf("\n");  //prompt to next line
    }
}
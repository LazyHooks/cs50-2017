
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include<stdlib.h>

int main(int argc, string argv[])
{
    if(argc!=2)
    {
        printf("Use ./crypt hash");
        return 1;
    }
   
    string hash="";
  
    strncpy(hash,argv[1]+2,11);
   
    printf("%s",hash);
    
   
    
}

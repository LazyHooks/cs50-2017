#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include<stdlib.h>

char rotateUpperCase(char ch, int k);
char rotateLowerCase(char ch, int k);


int main(int argc, string argv[])
{
    if(argc!=2)
    {
        printf("Usage: ./caesar k \n");
        return 1;
    }
   
   int k = atoi(argv[1]);
   k=k%26;
   printf("plaintext: ");
   string pt=get_string();
   printf("ciphertext: ");
   
   for(int i=0, n=strlen(pt);i<n;i++)     // to run a loop to pick out each individual character
   {
       
       int c=(int)pt[i];
       
       if(c>=65 && c<=90)
       {
          printf("%c", rotateUpperCase(pt[i],k));
       }
       else if(c>=97 && c<=122)
       {
          printf("%c", rotateLowerCase(pt[i],k));
       }
       else
       {
           printf("%c",pt[i]);
       }
   }
   
   printf("\n");
   return 0;
}

char rotateUpperCase(char ch, int k)
{
    int p=(int)ch-64;              // to treat A as 1 and so on
    if((p+k)!=26)                  // as 26 mod 26==0
    {
    p=(p+k)%26;                    // caesar's formula
    return (char)(64+p);           // adds 64 to restore it to ascii property
    }
    else
    {
        return (char)(64+26);      // if p+k=26, returns ascii value for Z
    }
}

char rotateLowerCase(char ch, int k)   
{ 
   int p=(int)ch-96;               // to treat a as 1 and so on
   if((p+k)!=26)
   {
    p=(p+k)%26;                    //caesar's formula
    return (char)(96+p);           // adds 96 to restore it to ascii property
   }
   else
   {
       return (char)(96+26);       // if p+k=26, returns ascii value for zcd 
   }
    
    
}
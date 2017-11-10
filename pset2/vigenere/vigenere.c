#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include<ctype.h>


char rotateUpperCase(char ch, int k);
char rotateLowerCase(char ch, int k);


int main(int argc, string argv[])
{
    if(argc!=2)  
    {
        printf("Usage: ./viginere k \n");
        return 1;
    }
    
    int counter=0;
    for(int i=0, n=strlen(argv[1]);i<n;i++)                 // to check if there are other characters than alphabets
    {
        if((int)argv[1][i]<65)
        {
            counter++;
        }
        if((int)argv[1][i]>122)
        {
            counter++;
        }
        if((int)argv[1][i]>90 && (int)argv[1][i]<97)
        {
            counter++;
        }
        }
        
        if(counter>0)                                       // if counter=0, there are only alphabets in the argument
        {
            printf("Usage: ./vigenere k \n");
            return 1;
        }
    
   
   printf("plaintext: ");
   string pt=get_string();
   printf("ciphertext: ");
   int v=0;
   for(int i=0, n=strlen(pt);i<n;i++)     // to run a loop to pick out each individual character
   {
       if(v==strlen(argv[1]))
       {
           v=0;
       }
       int k=0;
       
       if(isupper(argv[1][v]))
       {
           k=(int)argv[1][v]-65;
       }
       else if(islower(argv[1][v]))
       {
           k=(int)argv[1][v]-97;
       }
       
       
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
           v--;
       }
      
       v++;
   }
   
   printf("\n");
   return 0;
}

char rotateUpperCase(char ch, int k)
{
    int p=(int)ch-64;              // to treat A as 1 and so on
    if((p+k)!=26)                  // as 26 mod 26 would return 0
    {
    p=(p+k)%26;                    // vigenere formula
    return (char)(64+p);           // adds 64 to restore it to ascii property
    }
    else
    {
        return (char)(64+26);      // if p+k=26, returns Z
    }
}

char rotateLowerCase(char ch, int k)   
{ 
   int p=(int)ch-96;               // to treat a as 1 and so on
   if((p+k)!=26)                   // as 26 mod 26 would return 0
   {
    p=(p+k)%26;                    //vigenere formula
    return (char)(96+p);           // adds 96 to restore it to ascii property
   }
   else
   {
       return (char)(96+26);       //// if p+k=26, returns z
   }
    
    
}
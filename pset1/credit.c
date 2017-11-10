#include <stdio.h>
#include <cs50.h>

int addDigits(int n); // function to add digits of any number

int main(void)
{
    printf("Number:");
    long long n=get_long_long();
    long long n2=n;
    int sum=0;
    int counter=0;  // to keep check of number of digits in card no. entered by user
    while(n2>0)       // loop to get summation as per Luhn's formula
    {
        sum=sum+n2%10;              // adds digits we dont have to double
        n2=n2/10;                      
        sum=sum+addDigits((n2%10)*2);  // adds the digits of double 
        n2=n2/10;
        counter=counter+2;
    }
    
    for(int i=0;i<counter-3;i++)   // to get first two digits of card no.
    {
        n=n/10;
    }
    
    if(n>99)                       // failsafe in case of card no.s of even lengths
    {
        n=n/10;
    }
    
    if(sum%10==0)                // conditional output that is required
    {
        
        if(n>50 && n<56)
        {
            printf("MASTERCARD\n");
        }
        else if(n==34 || n==37)
        {
            printf("AMEX\n");
        }
        else if(n/10==4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}

int addDigits(int n)
{
    int s=0;
    while(n>0)
    {
        s=s+n%10;
        n=n/10;
    }
    return s;
    
}
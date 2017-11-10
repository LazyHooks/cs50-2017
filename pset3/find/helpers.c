/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if(n<0)            //returns false immediatelty if n is negative
    {
        return false;
    }
    
           
    int posL=0;              // default search lower limit is 0
    int posU=n-1;            // default upper limit of search range is last block of array
    int posM=(posU+posL)/2;  // posM = middle position of range
    while(posL<=posU)        //the begening of range cant be more than the ending
    {
        if(values[posM]==value)
        {
        return true;
        }
        else if(values[posM]<value)
        {
            posL=posM+1;       //if middle value of array is leser than needle, needle is in second half of haystack
        }
        else
        {
            posU=posM-1;       //if middle value is greater than needle, needle is in first half of haystack
        }
        posM=(posL+posU)/2;  //updates middle position
    }
    
    return false;            //if true is not already returned (value not found), returns false
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int pos=0;                    // stores position of where the sorting swap is going to take place
    for(int i=0;i<65536;i++)      // runs loop from 0 to max +ve value an integer can hold, to sort all integers in ascending order
    {
        for(int j=0;j<n;j++)      // runs loop through supplied array
        {
            if(values[j]==i)
            {
                int t=values[pos];     // if array value at j matches with i, it gets swapped to sorting position
                values[pos]=values[j];
                values[j]=t;
                pos++;
            }
        }
    }
    return;
}

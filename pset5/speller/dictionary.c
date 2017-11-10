/**
 * Implements a dictionary's functionality.
 */

#define HASHTABLE_CAPACITY 65536

#include <stdbool.h>

#include "dictionary.h"

#include <stddef.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

int hash_it(char* needs_hashing); //prototype

// define a node struct 
typedef struct node

{

    char word[LENGTH + 1];       //+1 for adding /0

    struct node* next;

}

node;


node* hashtable[HASHTABLE_CAPACITY]; //Hashtable of linked lists

//global variables for tracking operations
unsigned int word_count = 0;
bool loaded = false;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int wordlen= strlen(word); 
    char word_2[wordlen+1]; // word is a const char* and a copy is made so we can make changes to the copy
    
    for(int i=0;i<=wordlen;i++)       //including wordlen as we want to copy the \0 character too
    {
        word_2[i]= tolower(word[i]);  //copies word in lower case
    }
    
    
    
    int hashvalue = hash_it(word_2); //hash value stored 
    node* position = hashtable[hashvalue];
    
   // scan through link list at hashvalue in hastable
    while(position!=NULL)
    {
        if(strcmp(position->word,word_2)!=0)
        {
            //check next node
            position=position->next;
        }
        else
        {
            //word is in dictionary
            return true;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //reset all hashtable elements
    for(int j=0;j<HASHTABLE_CAPACITY;j++)
    {
        hashtable[j]=NULL;
    }
    
    FILE* fp=fopen(dictionary,"r");
    if(fp==NULL)
    {
        printf("Can't open dictionary");
        return false;
    }
    
    for(;;)
    {
        //malloc new node
        node* new= malloc(1*sizeof(node));
        if(new==NULL)
        {
            printf("malloc failed \n");
            return false;
        }
        
        fscanf(fp,"%s",new->word); //reads 1 word from a dictionary
        new->next=NULL;
        
        if(feof(fp)) //hits end of file
        {
            free(new);
            break;
        }
        
        word_count++;
        int hashvalue = hash_it(new->word);
        node* head = hashtable[hashvalue]; // pointer to the first node of the list
        
        if(head==NULL) //if bucket is empty, insertion of new node
        {
            hashtable[hashvalue]=new;
        }
        else //attach node to front of the list and make it the head
        {
            new->next = hashtable[hashvalue];
            hashtable[hashvalue]=new;
        }
    }
    
    fclose(fp);
    loaded=true;
    return true;
   
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (loaded)

    {

        return word_count;

    }

    else

    {

        return 0;

    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < HASHTABLE_CAPACITY; i++)

    {

        node* position = hashtable[i];

        while (position!= NULL)

        {

            node* trav = position; // holds connection to list

            position = position->next;

            free(trav);

        }

    }

    loaded = false;   //signals unloaded dictionary

    return true;
}

// hash function credits : https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn
int hash_it(char* needs_hashing)
{

    unsigned int hash = 0;

    for (int i=0, n=strlen(needs_hashing); i<n; i++)

        hash = (hash << 2) ^ needs_hashing[i];

    return hash % HASHTABLE_CAPACITY;

}
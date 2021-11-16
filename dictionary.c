// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <cs50.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

int counter = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //creat a node to iterate through the hash table
    node *cursor1 = table[hash(word)];

    //if the hashed value doesnt point to null, see if the hash table has the same word as the text
    while (cursor1 != NULL)
    {
        
        if (strcasecmp(cursor1->word, word) == 0)
        {
            return true;
        }
      
        //keep moving through the has table
        cursor1 = cursor1->next;
    }
    //free the allocated memory
    free(cursor1);
    return false;
}

// Hashes word into a number
unsigned int hash(const char *word)
{

    //djb2 hash function cited from https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f
    unsigned long hash = 5381;
    int c;


    while ((c = *word++))
    {
        //convert c to case insensitive word
        c = tolower(c);

        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    //return has number according to the number of buckets
    return hash % N;

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //start setting whole table elements to null
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    //create a node pointer and a word to copy words from
    node *n;
    char wordc[45];

    //open the file
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        printf("file not found\n");

        return 1;
    }
    
    //start scanning the file
    while (fscanf(file, "%s", wordc) != EOF)
    {
        //add word to the node by allocating memory
        n = malloc(sizeof(node));
         
        if (n == NULL)
        {
            return 1;
        }

        //copy the original word into the next node.
        strcpy(n->word, wordc);

        //set the next pointer to point nothing
        n->next = NULL;

        //if there is no saved word in the table let it point to copyed word
        if (table[hash(wordc)] == NULL)
        {
            table[hash(wordc)] = n;
        }

        //else make the head point to table first then make table point to new word
        else
        {
            n->next = table[hash(wordc)];
            table[hash(wordc)] = n;
        }
        
        //set counter to count words
        counter++;

    }
    
    //close the dictionary
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //return the number of words
    if (counter >= 1)
    {
        return counter;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //create 2 nodes to point to table nodes side by side
    node *cursor;
    node *tmp;
    //iterate through whole table to free all allocated memory
    for (int i = 0; i < N; i++)
    {
        //setting cursor to head first
        cursor = table[i];
        
        //no need to free unused table elemnt
        if (table[i] != NULL)
        {
            
            //itirate through all linked list until the next pointer is null
            while (cursor->next != NULL)
            {
                //set temprary pointer to cursor and after moving the cursor free the allocated memory
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
    
        //free the cursor itself
        free(cursor);
       
        //return true if the whole table is freed.
        if (i == N - 1)
        {
            return true;
        }
        
    }
    
 
    
    return false;
}

// $Id: hashset.c,v 1.7 2013-05-23 15:41:07-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "debugf.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   for(size_t i = 0; i < this->size; ++i)
   {
      this->chains[i] = NULL;
   }
   DEBUGF ('h', "%p -> struct hashset {size = %d, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void free_hashset (hashset *this) {
   DEBUGF ('h', "free (%p)\n", this);
   //free memory 
   for(size_t i = 0; i < this->size; i++)
   {
      hashnode *curr = this->chains[i];
      while(curr != NULL) {
         hashnode *temp = curr;
         curr = curr->link;
         free(temp->word);
         free(temp);
      }
   }
   memset(this->chains, 0, this->load * sizeof(hashnode *));
   free(this->chains);
   memset(this, 0, sizeof(struct hashset));
   free(this);
}

void put_hashset (hashset *this, char *item) {
   //STUBPRINTF ("hashset=%p, item=%s\n", this, item);
   if((this->load * 2) > this->size) {
      doublesize (this);
   }
    
   hashcode_t index = strhash(item) % this->size;
   if(this->chains[index] == NULL)
   {
      this->chains[index] = malloc(sizeof(struct hashnode));
      this->chains[index]->word = item;
      this->chains[index]->link = NULL;
   }else{
      hashnode *current = this->chains[index];
      hashnode *prev = NULL;
      while(current != NULL){
         if(strcmp(current->word,item) == 0) return;
         prev = current;
         current = current->link;
      }
      hashnode *temp = malloc(sizeof (struct hashnode));
      temp->word = item;
      temp->link = NULL;
      prev->link = temp;
   }
   ++this->load;
}

void doublesize (hashset *this) {
   size_t old = this->size;
   this->size = (old * 2) + 1;
   hashnode **new = malloc(this->size * sizeof (hashnode *));
   for(size_t i = 0; i < this->size; i++)
   {
      new[i] = NULL;
   }
   for(size_t i = 0; i < old; i++)
   {
      if(this->chains[i] == NULL) continue;
      hashnode *pop = this->chains[i];
      while(pop != NULL) {
         hashcode_t index = strhash(pop->word)%this->size;
         hashnode *current = new[index];
         hashnode *prev = NULL;
         if(current == NULL) {
            new[index] = pop;
            pop = pop->link;
            new[index]->link = NULL;
         } else{
            while(current != NULL) {
               prev = current;
               current = current->link;
            }
            prev->link = pop;
            pop = pop->link;
            prev->link->link = NULL;
         }
      }
   }
   hashnode **temp = this->chains;
   this->chains = new;
   free(temp);
}

bool has_hashset (hashset *this, char *item) {
   //STUBPRINTF ("hashset=%p, item=%s\n", this, item);
   bool found = false;
   char *lower = strdup(item);
   
   for(int i = 0; lower[i] != '\0'; i++) {
      lower[i] = tolower(lower[i]);
   }
   hashcode_t index1 = strhash(lower) % this->size;
   hashcode_t index2 = strhash(item) % this->size;
   for(hashnode *curr = this->chains[index1]; 
       curr != NULL; curr = curr->link)
   {
      if(strcmp(curr->word, lower) == 0) found = true;
   }
   for(hashnode *curr = this->chains[index2];
       curr != NULL; curr = curr->link)
   {
      if(strcmp(curr->word, item) == 0) found = true;
   }
   free(lower);
   return found;
}

void debugcheck (hashset *this, int debug) {
   printf("%10d words in the hash set\n", (int) this->load);
   printf("%10d size of the hash array\n", (int) this->size);
   
   int chainsize[this->load];

   for(size_t i = 0; i < this->load; i++) {
      chainsize[i] = 0;
   }   
   
   int count = 0;
   for(size_t i = 0; i < this->size; i++) {

      if(this->chains[i] != NULL) {
         for(hashnode *curr = this->chains[i]; curr != NULL;
             curr = curr->link){
             ++count;
         }
         ++chainsize[count];
         count = 0;
      }
         
   }
   
   for (size_t i = 1; i < this->load; i++) {
      if (chainsize[i] != 0) {
         printf("%10d chains of size %i\n", chainsize[i] , (int)i);
      }
   }

   if(debug > 1) {
      for(size_t i = 0; i < this->size; i++) 
      {
         hashnode *current = this->chains[i];
         while(current != NULL) 
         {
            hashcode_t hash = strhash(current->word);
            if(current == this->chains[i])
            {
               printf("array[%10d] = %12u \"%s\"\n", (int) i,
                      hash, current->word);
            } else {
               printf("                  = %12u \"%s\"\n", hash,
                      current->word);
            }
            current = current->link;
         }
      }
   }
}

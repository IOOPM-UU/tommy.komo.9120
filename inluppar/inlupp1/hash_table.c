#include <stdlib.h>
#include "hash_table.h"
#include <stdio.h>
#include <stdbool.h>


typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;


struct entry
{
    int key;
    char *value; 
    entry_t *next;
};

struct hash_table
{
    entry_t buckets[17];
};


static entry_t *entry_create(int key, char *value, entry_t *next)
{
  entry_t *created_entry = calloc(1,sizeof(entry_t));

  created_entry->key = key;
  created_entry->value = value;
  created_entry->next = next;
  
  return created_entry;
}

static void entry_destroy(entry_t *entry){
  free(entry);
}

static entry_t *find_previous_entry_for_key(entry_t *bucket,int key){
  
  entry_t *cursor = bucket;
  
  while(cursor -> next != NULL){
      if(cursor->next->key >= key)
      {
        return cursor;
      } 
       else 
      {
        cursor = cursor->next;
      }
    }
    return cursor; 
}

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  return result;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  
  for(int i = 0; i < 17; i++){      // Loopar igenom buckets        
    entry_t *cursor = ht->buckets[i].next; // cursor pekar mot första riktiga entryn 
    if(cursor == NULL){            // kollar att första riktiga inte är NULL, om så är fallet gå till nästa bucket
      continue;
    }
    while(cursor != NULL){
    entry_t *next = cursor->next;  // Sparar pointern till nästa entry i bucketen
    entry_destroy(cursor);                // Free den entry vi tittar på just nu
    cursor = next;                  // Skriver om cursor till att peka mot nästa entry         // Kör loopen igen tills nästa entry är NULL
    }    
  }
  free(ht);
}


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value) 
{
  /// Calculate the bucket for this entry
  int bucket = ((key % 17) + 17) % 17;
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && next->key == key)
    {
      next->value = value;
    }
  else
    {
      entry->next = entry_create(key, value, next);
    }
}


bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, char **result){   //skickar med en pekare till en pekare, eftersom när vi hittat value som är kopplad till den nyckel vi söker för, ska ge tillbaka en bool för att signalera det som en pointer som pekar mot det värdets plats i minnet. Den pekaren kommer vi lagra i char *val. när vi använder funktionen lookup skickar vi med &val, alltså adressen till val men val är ju redan en pekare så det är att skicka adressen till en pekare så att man kan ändra själva pekaren. char ** med andra ord.
  int bucket = ((key % 17) + 17) % 17;
  /// Find the previous entry for key
  entry_t *tmp = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *next = tmp->next;

if (next && next->key == key) 
  {
    *result = next->value;  // kan va null och det är fine eftersom vi returnerat true också 
    return true;
  }
else
  {
    *result = NULL;
    return false;
  }
}


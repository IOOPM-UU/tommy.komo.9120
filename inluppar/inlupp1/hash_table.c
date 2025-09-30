#include <stdlib.h>
#include "hash_table.h"
#include <stdio.h>

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



entry_t *entry_create(int key, char *value, entry_t *next)
{
  entry_t *created_entry = calloc(1,sizeof(entry_t));

  created_entry->key = key;
  created_entry->value = value;
  created_entry->next = next;
  
  return created_entry;
}

entry_t *find_previous_entry_for_key(entry_t *bucket,int key){
  
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
  free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value) 
{
  /// Calculate the bucket for this entry
  int bucket = key % 17;
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

char *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key){
    return (NULL);
}


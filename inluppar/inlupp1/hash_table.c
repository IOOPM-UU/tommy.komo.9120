#include <stdlib.h>
#include "hash_table.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define No_Buckets 17

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef bool ioopm_predicate(int key, char *value, void *extra);
typedef void ioopm_apply_function(int key, char **value, void *extra);


struct entry
{
    int key;
    char *value; 
    entry_t *next;
};

struct hash_table
{
    entry_t buckets[No_Buckets];
};


static int bucket_index(int key)
{
  return((key % No_Buckets) + No_Buckets) % No_Buckets;
}

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

static bool key_equiv(int key, char *value_ignored, void *x)
{
  int *other_key_ptr = x;
  int other_key = *other_key_ptr;
  return key == other_key;
}

static bool value_equiv(int key_ignored, char *entry_value, void *x)
{
  (void)key_ignored;
  char *other_value = x;
  return strcmp(entry_value, other_value) == 0;
}

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  return result;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  
  for(int i = 0; i < No_Buckets; i++){      // Loopar igenom buckets        
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
  int bucket = bucket_index(key);
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
  int bucket = bucket_index(key);
  /// Find the previous entry for key
  entry_t *tmp = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *next = tmp->next;

if (next && next->key == key) 
  {
    if (result) *result = next->value;  // kan va null och det är fine eftersom vi returnerat true också 
    return true;
  }
else
  {
    if (result) *result = NULL;
    return false;
  }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key, char **removed_value){
  int bucket = bucket_index(key);

  entry_t *prev = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *cursor = prev->next;

  if (cursor && cursor->key == key)
    {
      if (removed_value) *removed_value = cursor->value;  //användaren kan själv välja om den vill ta bort 
      prev->next = cursor->next;
      free(cursor);
      return true;
    }
  else
    {
      if (removed_value) *removed_value = NULL;
      return false;
    }
  }

int ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  int counter = 0;
  
  for(int bucket_count = 0; bucket_count < No_Buckets; bucket_count++)
  {
    entry_t *bucket = &ht->buckets[bucket_count];
    entry_t *next_entry = bucket->next;    
  
    while (next_entry != NULL)
    {
      counter++;
      next_entry = next_entry->next;
    }
  } return counter;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for(int bucket_count = 0; bucket_count < No_Buckets; bucket_count++)
  {
    entry_t *bucket = &ht->buckets[bucket_count];
    entry_t *next_entry = bucket->next; // samma som innan men stoppar så fort det finns en entry
    if(next_entry != NULL){
      return false;
    }
  } return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht){
    
  for(int i = 0; i < No_Buckets; i++){      // Loopar igenom buckets        
    entry_t *cursor = ht->buckets[i].next; // cursor pekar mot första riktiga entryn 
    if(cursor == NULL){            // kollar att första riktiga inte är NULL, om så är fallet gå till nästa bucket
      continue;
    }
    while(cursor != NULL){
    entry_t *next = cursor->next;  // Sparar pointern till nästa entry i bucketen
    entry_destroy(cursor);                // Free den entry vi tittar på just nu
    cursor = next;                  // Skriver om cursor till att peka mot nästa entry         // Kör loopen igen tills nästa entry är NULL
    }
    ht->buckets[i].next = NULL;     //Detta för att den nu tomma bucketen inte ska ha en dangling pointer. Det vi gör är bara att sätta dummyns next till null så den omöjligt kan peka på skräp
  }
}

int *ioopm_hash_table_keys(ioopm_hash_table_t *ht){
  int counter = 0;
  int size = ioopm_hash_table_size(ht);
  int *all_keys = calloc(size, sizeof(int)); // Detta för att få en tom array att kunna lagra nycklarna i
 
  for(int i = 0; i < No_Buckets; i++)
  {
    entry_t *bucket = &ht->buckets[i];
    entry_t *next_entry = bucket->next;    //dessa är kopierade från tidigare lösningar och ger oss helt enkelt bara bucketsarna i omvänd ordning
    
    while (next_entry != NULL)
    {
      all_keys[counter] = next_entry->key;
      counter++;
      next_entry = next_entry->next;
    }
  } return all_keys;
}

char **ioopm_hash_table_values(ioopm_hash_table_t *ht){
  int counter = 0;
  int size = ioopm_hash_table_size(ht);
  char **all_values = calloc(size, sizeof(char *)); // Detta för att få en tom array att kunna lagra values i
 
  for(int i = 0; i < No_Buckets; i++)
  {
    entry_t *bucket = &ht->buckets[i];
    entry_t *next_entry = bucket->next;  
    
    while (next_entry != NULL)
    {
      all_values[counter] = next_entry->value;
      counter++;
      next_entry = next_entry->next;
    }
  } return all_values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, int key){

  return ioopm_hash_table_any(ht, key_equiv, &key);
}


bool ioopm_hash_table_has_values(ioopm_hash_table_t *ht, char *value)
{
   return ioopm_hash_table_any(ht, value_equiv, value);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *extra)
{
  for(int i = 0; i < No_Buckets;i++){
    entry_t *bucket = &ht->buckets[i];
    entry_t *cursor = bucket->next;

    while(cursor != NULL){
      if (pred(cursor->key, cursor->value, extra)){
        return true;
      }
      cursor = cursor->next;
    }
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *extra)
{
  for(int i = 0; i < No_Buckets; i++){
    entry_t *bucket = &ht->buckets[i];
    entry_t *cursor = bucket->next;

    while(cursor != NULL){
      if (!pred(cursor->key, cursor->value, extra)){
        return false;
      }
      cursor = cursor->next;
    } 
  }
  return true;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_func, void *arg)
{
  for(int i = 0; i < No_Buckets; i++){
    entry_t *bucket = &ht->buckets[i];
    entry_t *cursor = bucket->next;
  
    while(cursor != NULL){
      apply_func(cursor->key, &cursor->value, arg);
      cursor = cursor->next;
    }
  }
}
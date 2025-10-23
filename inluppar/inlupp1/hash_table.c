#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"


#define No_Buckets 17

typedef struct entry entry_t;


struct entry
{
    elem_t key;
    elem_t value; 
    entry_t *next;
};

struct hash_table
{
    entry_t buckets[No_Buckets];
    ioopm_hash_function *hash_fun;
    ioopm_eq_function *key_eq;
    ioopm_eq_function *val_eq;
};


static size_t bucket_index(ioopm_hash_table_t *ht, elem_t key)
{
  size_t h = ht->hash_fun ? ht->hash_fun(key) : ioopm_extract_int_hash(key);  // Med hjälp av GPT
  return h % No_Buckets;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
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

static entry_t *find_entry(entry_t *bucket, ioopm_eq_function *key_eq, elem_t key)
{
  for (entry_t *e = bucket->next; e; e = e->next)  // Första riktiga noden, jämförs med hjälp av key eq sedan stegas igenom hela bucketen
    if (key_eq(e->key, key)) return e;
  return NULL;
} 

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fun, ioopm_eq_function *key_eq, ioopm_eq_function *val_eq)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->hash_fun = hash_fun ? hash_fun : ioopm_extract_int_hash;  // alltså om användaren gett en hash function använt den, annars defaulta till våran.
  ht->key_eq = key_eq;
  ht->val_eq = val_eq;
  return ht;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  
  for(size_t i = 0; i < No_Buckets; i++){      // Loopar igenom buckets        
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


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) 
{
  
  size_t bucket = bucket_index(ht, key);
  
  entry_t *entry = find_entry(&ht->buckets[bucket], ht->key_eq, key);
  if (entry){
    entry->value = value;
  }
  else 
  {
    ht->buckets[bucket].next = entry_create(key, value, ht->buckets[bucket].next);  //uppdaterar dummy node till att peka på vår nya entry, och vår entry pekar i sin tur på förra entryn som vår första efter dummyn.
  }
}


bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result){   //skickar med en pekare till en pekare, eftersom när vi hittat value som är kopplad till den nyckel vi söker för, ska ge tillbaka en bool för att signalera det som en pointer som pekar mot det värdets plats i minnet. Den pekaren kommer vi lagra i char *val. när vi använder funktionen lookup skickar vi med &val, alltså adressen till val men val är ju redan en pekare så det är att skicka adressen till en pekare så att man kan ändra själva pekaren. char ** med andra ord.
  size_t bucket = bucket_index(ht, key);
  entry_t *entry = find_entry(&ht->buckets[bucket], ht->key_eq, key);
  if (entry){
    if (result) *result = entry->value;
    return true;
  }
  else
  {
    if (result) *result = (elem_t){0};
    return false;
  }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *removed_value){
  size_t bucket = bucket_index(ht, key);

  entry_t *prev = &ht->buckets[bucket];
  entry_t *cursor = prev->next;

  while (cursor && !ht->key_eq(cursor->key, key)) {
    prev = cursor; 
    cursor = cursor->next;
  }
  if (cursor)
  {
    if (removed_value) *removed_value = cursor->value;
    prev->next = cursor->next;
    free(cursor);
    return true;
  }
  else
    {
      if (removed_value) *removed_value = (elem_t){0};
      return false;
    }
  }

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t counter = 0;
  
  for(size_t bucket_count = 0; bucket_count < No_Buckets; bucket_count++)
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
  for(size_t bucket_count = 0; bucket_count < No_Buckets; bucket_count++)
  {
    entry_t *bucket = &ht->buckets[bucket_count];
    entry_t *next_entry = bucket->next; // samma som innan men stoppar så fort det finns en entry
    if(next_entry != NULL){
      return false;
    }
  } return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht){
    
  for(size_t i = 0; i < No_Buckets; i++){      // Loopar igenom buckets        
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

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht){
  
  ioopm_list_t *lnk_list = ioopm_linked_list_create(NULL);
 
  for(size_t i = 0; i < No_Buckets; i++)
  {
    entry_t *bucket = &ht->buckets[i];
    entry_t *next_entry = bucket->next;    //dessa är kopierade från tidigare lösningar och ger oss helt enkelt bara bucketsarna i omvänd ordning
    
    while (next_entry != NULL)
    {
      ioopm_linked_list_append(lnk_list, next_entry->key);
      next_entry = next_entry->next;
    }
  } return lnk_list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht){
  ioopm_list_t *values = ioopm_linked_list_create(NULL);
  
  for(size_t i = 0; i < No_Buckets; i++)
  {
    for(entry_t *e = ht->buckets[i].next; e; e = e->next){
      ioopm_linked_list_append(values, e->value);
    }
  } 
  return values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key){

  size_t bucket = bucket_index(ht, key);
  bool answer = find_entry(&ht->buckets[bucket], ht->key_eq, key) != NULL;
  return answer;
}


bool ioopm_hash_table_has_values(ioopm_hash_table_t *ht, elem_t value)
{
   for(size_t i = 0; i < No_Buckets;i++){
    entry_t *entry = ht->buckets[i].next;

    while (entry != NULL){
      bool are_eq;
      if(ht->val_eq != NULL){
        are_eq = ht->val_eq(entry->value, value);
      }
      else{
        are_eq = (entry->value.p == value.p && entry->value.i == value.i);
      }
      if(are_eq){
        return true;
      }
      entry = entry->next;
    }

   }
   return false;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *extra)
{
  for(size_t i = 0; i < No_Buckets;i++){
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
  for(size_t i = 0; i < No_Buckets; i++){
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
  for(size_t i = 0; i < No_Buckets; i++){
    entry_t *bucket = &ht->buckets[i];
    entry_t *cursor = bucket->next;
  
    while(cursor != NULL){
      apply_func(cursor->key, &cursor->value, arg);
      cursor = cursor->next;
    }
  }
}
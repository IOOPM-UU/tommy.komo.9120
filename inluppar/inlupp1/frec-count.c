#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "iterator.h"
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
  // FIXME: Rewrite to match your own interface, error-handling, etc. Tror jag löste detta rätt, vi behöve ändra eftersom vår lookup inte returnerar ett värde utan en bool men vi kan fof utvinna värdet
  elem_t value;
  int freq = 0;
  if(ioopm_hash_table_lookup(ht, (elem_t) {.p = word}, &value)){
    freq = value.i;
    ioopm_hash_table_insert(ht, (elem_t) {.p = word}, (elem_t) {.i = freq + 1});//vi behöver inte göra en ny nyckel med strdup om den redan finns
  }else{
    ioopm_hash_table_insert(ht, (elem_t) {.p = strdup(word)}, (elem_t) {.i = freq + 1});
  }
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    size_t read = getline(&buf, &len, f);//hämtar hela raden och lägger den i buf

    if (read == -1)//feof kollar om man har nått "end of file"
    {
      free(buf);
      break;
    }

    for (char *word = strtok(buf, Delimiters);//strtok delar upp alla rader i ord
         word && *word;//word är en pekare till ordet så om man är på sista ordet kommer den vara = NULL och loopen bryts, *word betyder innehållet i första charen av ordet och om den pekar på en tom sträng så kommer *word == '\0' vilket är falskt
         word = strtok(NULL, Delimiters))//fortsätter på nästa ord i forlopen
    {
      process_word(word, ht);
    }

    free(buf);
  }

  fclose(f);
}

size_t string_sum_hash(elem_t e)
{
  char *str = e.p;
  size_t result = 0;
  while(*str) result += *str++;
  return result;
}

bool string_eq(elem_t e1, elem_t e2)
{
  return (strcmp(e1.p, e2.p) == 0);
}



int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_sum_hash, string_eq, NULL);
  
  char **keys = NULL;
  ioopm_list_iterator_t *iter = NULL;

  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }

    // FIXME: If the keys are returned as a list, transfer them into
    // an array to use `sort_keys` (perhaps using an iterator?)
    ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
    int size = ioopm_hash_table_size(ht);

    keys = malloc(size * sizeof(char *));
    iter = ioopm_list_iterator(keys_list);

    int counter = 0;

    while(ioopm_iterator_has_next(iter)) {
        keys[counter++] = ioopm_iterator_next(iter).p;
    }   



    sort_keys(keys, size);

    for (int i = 0; i < size; ++i)
    {
      // FIXME: Update to match your own interface, error handling, etc.
      elem_t value;
      if(ioopm_hash_table_lookup(ht, (elem_t) {.p = keys[i]}, &value)){
      printf("%s: %d\n", keys[i], value.i);
      }
    }
  }
  else
  {
    puts("Usage: freq-count file1 ... filen");
  }

  // FIXME: Leaks memory! Use valgrind to find out where that memory is
  // being allocated, and then insert code here to free it.
  ioopm_iterator_destroy(iter);
  free(keys);
  ioopm_hash_table_destroy(ht);
}

#include <CUnit/Basic.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"


static size_t int_hash(elem_t k) { return (size_t)k.i; }
static bool   int_eq(elem_t a, elem_t b) { return a.i == b.i; }
static bool   str_eq(elem_t a, elem_t b)
{
  const char *sa = a.p, *sb = b.p;
  if (sa == NULL || sb == NULL) return sa == sb;
  return strcmp(sa, sb) == 0;
}

#define HT() ioopm_hash_table_create(int_hash, int_eq, str_eq)


int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

int init_suite_list(void)
{ 
    return 0; 
}

int clean_suite_list(void)
{ 
    return 0; 
}

int init_suite_iterator(void)
{ 
    return 0; 
}

int clean_suite_iterator(void)
{ 
    return 0; 
}





void test_lookup_empty()
{
  ioopm_hash_table_t *ht = HT();

  for (int i = 0; i < 18; ++i) { /// 18 is chosen due to our 17 pre-chosen buckets that can't be changed
    elem_t v = {0};
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(i), &v));
    CU_ASSERT_PTR_NULL(v.p);
  }

  ioopm_hash_table_destroy(ht);
}

void test_create_destroy()
{
  ioopm_hash_table_t *ht = HT();
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}


void test_update_existing_key()
{
  // Testar att ändra värdet en redan existerande nyckel (använder värdet från förra testet)
  
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("test"));
  ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("rätt"));

  elem_t keyvalue = {0};
  bool truefalse = ioopm_hash_table_lookup(ht, int_elem(5), &keyvalue);
  CU_ASSERT_TRUE(truefalse);
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "rätt");

  ioopm_hash_table_destroy(ht);
}

void test_collision_same_bucket()
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5),  ptr_elem("a"));
  ioopm_hash_table_insert(ht, int_elem(22), ptr_elem("b"));

  elem_t keyvalue = {0};
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(22), &keyvalue));
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "b");

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(5), &keyvalue));
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "a");

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1), &keyvalue));

  ioopm_hash_table_destroy(ht);
}

void test_insert_lookup()
{
  ioopm_hash_table_t *ht = HT();
  ioopm_hash_table_insert(ht, int_elem(32), ptr_elem("hello"));

  elem_t keyvalue = {0};
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(32), &keyvalue));
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "hello");

  ioopm_hash_table_destroy(ht);
}


void test_key_in_use()
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(7), ptr_elem("test"));
  ioopm_hash_table_insert(ht, int_elem(7), ptr_elem("correct"));

  elem_t keyvalue = {0};
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(7), &keyvalue));
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "correct");

  ioopm_hash_table_destroy(ht);
}

void test_no_key_lookup()
{
  ioopm_hash_table_t *ht = HT();
  ioopm_hash_table_insert(ht, int_elem(7), ptr_elem("test"));

  elem_t keyvalue = {0};
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(8), &keyvalue));

  ioopm_hash_table_destroy(ht);
}

void test_negative_keys()
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(-7), ptr_elem("minusseven"));

  elem_t keyvalue = {0};
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(-7), &keyvalue));
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "minusseven");

  ioopm_hash_table_destroy(ht);
}

void test_null_value(void)
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(1), ptr_elem(NULL));

  elem_t keyvalue = ptr_elem("dummy");
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(1), &keyvalue));
  CU_ASSERT_PTR_NULL(keyvalue.p);

  ioopm_hash_table_destroy(ht);
}

void test_extreme_keys(void)
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(INT_MAX), ptr_elem("max"));
  ioopm_hash_table_insert(ht, int_elem(INT_MIN), ptr_elem("min"));

  elem_t keyvalue = {0};
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(INT_MAX), &keyvalue));
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "max");

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(INT_MIN), &keyvalue));
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "min");

  ioopm_hash_table_destroy(ht);
}

void test_remove_entry_fail()
{
  ioopm_hash_table_t *ht = HT();

  bool truefalse = ioopm_hash_table_remove(ht, int_elem(32), NULL);
  CU_ASSERT_FALSE(truefalse);

  ioopm_hash_table_destroy(ht);
}

void test_remove_entry_success()
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(32), ptr_elem("hello"));

  elem_t keyvalue = {0};
  bool truefalse = ioopm_hash_table_remove(ht, int_elem(32), &keyvalue);

  CU_ASSERT_TRUE(truefalse);
  CU_ASSERT_STRING_EQUAL((char *)keyvalue.p, "hello");

  ioopm_hash_table_destroy(ht);
}

void test_count_entrys_empty()
{
  ioopm_hash_table_t *ht = HT();

  size_t x = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(x, 0);

  ioopm_hash_table_destroy(ht);
}

void test_count_one_entry()
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(32), ptr_elem("hello"));
  size_t x = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(x, 1);

  ioopm_hash_table_destroy(ht);
}

void test_count_entrys()
{
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5),  ptr_elem("h"));
  ioopm_hash_table_insert(ht, int_elem(0),  ptr_elem("e"));
  ioopm_hash_table_insert(ht, int_elem(30), ptr_elem("l"));
  ioopm_hash_table_insert(ht, int_elem(17), ptr_elem("l"));
  ioopm_hash_table_insert(ht, int_elem(6),  ptr_elem("o"));

  size_t x = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(x, 5);

  ioopm_hash_table_destroy(ht);
}

void test_hash_table_clear_and_empty(){
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5),  ptr_elem("h"));
  ioopm_hash_table_insert(ht, int_elem(0),  ptr_elem("e"));
  ioopm_hash_table_insert(ht, int_elem(30), ptr_elem("l"));
  ioopm_hash_table_insert(ht, int_elem(17), ptr_elem("l"));
  ioopm_hash_table_insert(ht, int_elem(6),  ptr_elem("o"));

  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_clear(ht);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_all_keys(){
  ioopm_hash_table_t *ht = HT();

  int keys[5] = {3, 10, 42, 0, 99};
  bool found[5] = {false};

  ioopm_hash_table_insert(ht, int_elem(3),  ptr_elem("h"));
  ioopm_hash_table_insert(ht, int_elem(10), ptr_elem("e"));
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("l"));
  ioopm_hash_table_insert(ht, int_elem(0),  ptr_elem("l"));
  ioopm_hash_table_insert(ht, int_elem(99), ptr_elem("o"));

  ioopm_list_t *table_keys = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(table_keys);

  while (ioopm_iterator_has_next(iter)) {
    elem_t e = ioopm_iterator_next(iter);
    bool match_found = false;
    for (int z = 0; z < 5; z++) { //loop för alla nycklar vi har
      
      if (keys[z] == e.i) 
      {
        found[z] = true;
        match_found = true;
        break;
      }
    }
    if (!match_found) CU_FAIL("Found a key that was never inserted!");
  }
  for (int i = 0; i < 5; i++) CU_ASSERT_TRUE(found[i]);

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(table_keys);
  ioopm_hash_table_destroy(ht);
}

void test_all_values()
{
  ioopm_hash_table_t *ht = HT();

  int   keys[5]   = {3, 10, 42, 0, 99};
  char *values[5] = {"three","ten","fortytwo","zero","ninetynine"};
  bool  found[5]  = {false,false,false,false,false};

  for (int i = 0; i < 5; i++) {
    ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i])); // lägg in korresponderande nyckel med värde 
  }

  ioopm_list_t *vals = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t *it = ioopm_list_iterator(vals);

  while (ioopm_iterator_has_next(it)) {
    elem_t e = ioopm_iterator_next(it);
    const char *got = e.p;
    bool match_found = false;
    for (int j = 0; j < 5; ++j) {
      if (strcmp(values[j], got) == 0) {
        found[j] = true;
        match_found = true;
        break;
      }
    }
    if (!match_found) CU_FAIL("Found a value that was never inserted!");
  }

  for (int i = 0; i < 5; i++) CU_ASSERT_TRUE(found[i]);

  ioopm_iterator_destroy(it);
  ioopm_linked_list_destroy(vals);
  ioopm_hash_table_destroy(ht);
}

void test_if_key_exist(){
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("hej"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(5)));
  ioopm_hash_table_insert(ht, int_elem(22), ptr_elem("då"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(5)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(22)));

  ioopm_hash_table_destroy(ht);
}

void test_if_value_exist(){
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("hej"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_values(ht, ptr_elem("hej")));

  char *copy = malloc(strlen("hej") + 1);
  strcpy(copy, "hej");
  CU_ASSERT_TRUE(ioopm_hash_table_has_values(ht, ptr_elem(copy)));
  free(copy);

  ioopm_hash_table_destroy(ht);
}

bool value_is_hej(elem_t key, elem_t value, void *x)
{
  (void)key; 
  (void)x;

  const char *v = value.p;
  return v && strcmp(v, "hej") == 0; //FÅR SEGFALUT UTAN "return v", kan användas vid redovisning
}

void test_pred_any(){
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5),  ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(22), ptr_elem("fel"));

  bool result = ioopm_hash_table_any(ht, value_is_hej, NULL);
  CU_ASSERT_TRUE(result);

  ioopm_hash_table_clear(ht);

  ioopm_hash_table_insert(ht, int_elem(22), ptr_elem("fel"));
  bool wrong_result = ioopm_hash_table_any(ht, value_is_hej, NULL);
  CU_ASSERT_FALSE(wrong_result);

  ioopm_hash_table_destroy(ht);
}

void test_pred_all(){
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(5),  ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(22), ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(25), ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(0),  ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(16), ptr_elem("hej"));

  bool result = ioopm_hash_table_all(ht, value_is_hej, NULL);
  CU_ASSERT_TRUE(result);

  ioopm_hash_table_clear(ht);

  ioopm_hash_table_insert(ht, int_elem(22), ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(25), ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(0),  ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(16), ptr_elem("hej"));
  ioopm_hash_table_insert(ht, int_elem(22), ptr_elem("fel"));

  bool wrong_result = ioopm_hash_table_all(ht, value_is_hej, NULL);
  CU_ASSERT_FALSE(wrong_result);

  ioopm_hash_table_destroy(ht);
}

void apply_string_to_all(elem_t key_ignore, elem_t *value, void *extra)
{
  (void)key_ignore;
  value->p = extra;
}

void test_apply_to_all()
{
  char *str = "paint";
  ioopm_hash_table_t *ht = HT();

  ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("green"));
  ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("blue"));
  ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("red"));

  ioopm_hash_table_apply_to_all(ht, apply_string_to_all, str);

  CU_ASSERT_TRUE (ioopm_hash_table_has_values(ht, ptr_elem("paint")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_values(ht, ptr_elem("green")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_values(ht, ptr_elem("blue")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_values(ht, ptr_elem("red")));

  ioopm_hash_table_destroy(ht);
}


void test_list_create_destroy(void)//alla linked_list funktions tester
{
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  CU_ASSERT_PTR_NOT_NULL(lnk_lst);
  ioopm_linked_list_destroy(lnk_lst);
}

void test_prepend(void)
{
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);

  ioopm_linked_list_prepend(lnk_lst, int_elem(3));

  CU_ASSERT_EQUAL(ioopm_linked_list_size(lnk_lst), (size_t)1);
  CU_ASSERT_FALSE(ioopm_linked_list_is_empty(lnk_lst));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 0).i, 3);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_append_and_get(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(4));
  ioopm_linked_list_append(lnk_lst, int_elem(5));

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 1).i, 5);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_insert_list_and_prepend(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);

  ioopm_linked_list_insert(lnk_lst, 0, int_elem(7));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 0).i, 7);

  ioopm_linked_list_append(lnk_lst, int_elem(4));
  ioopm_linked_list_insert(lnk_lst, 1, int_elem(8));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 1).i, 8);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_remove(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(3));

  ioopm_linked_list_remove(lnk_lst, 0);

  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lnk_lst));
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lnk_lst), 0);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_clear(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(1));
  ioopm_linked_list_append(lnk_lst, int_elem(2));
  ioopm_linked_list_append(lnk_lst, int_elem(3));

  ioopm_linked_list_clear(lnk_lst);

  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lnk_lst));
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lnk_lst), 0);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_contains(){
  ioopm_list_t *list = ioopm_linked_list_create(int_eq); 

  ioopm_linked_list_append(list, int_elem(10));
  ioopm_linked_list_append(list, int_elem(20));
  ioopm_linked_list_append(list, int_elem(30));

  CU_ASSERT_TRUE (ioopm_linked_list_contains(list, int_elem(20)));
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list, int_elem(99)));

  ioopm_linked_list_destroy(list);
}

static bool is_odd(elem_t value, void *extra) {
  (void) extra;
  return (value.i % 2) == 1;
}

void test_linked_list_all()
{
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(1));
  ioopm_linked_list_append(lnk_lst, int_elem(3));
  ioopm_linked_list_append(lnk_lst, int_elem(5));

  CU_ASSERT_TRUE(ioopm_linked_list_all(lnk_lst, is_odd, NULL));

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_any(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(2));
  ioopm_linked_list_append(lnk_lst, int_elem(4));

  CU_ASSERT_FALSE(ioopm_linked_list_any(lnk_lst, is_odd, NULL));

  ioopm_linked_list_append(lnk_lst, int_elem(5));
  CU_ASSERT_TRUE(ioopm_linked_list_any(lnk_lst, is_odd, NULL));

  ioopm_linked_list_destroy(lnk_lst);
}

static void add_n(elem_t *value, void *extra) {
  int *n = extra;
  value->i += *n;
}

void test_linked_list_apply_to_all(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);

  ioopm_linked_list_append(lnk_lst, int_elem(1));
  ioopm_linked_list_append(lnk_lst, int_elem(2));
  ioopm_linked_list_append(lnk_lst, int_elem(3));

  int nmr = 1;
  ioopm_linked_list_apply_to_all(lnk_lst, add_n, &nmr);

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 0).i, 2);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 1).i, 3);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 2).i, 4);

  ioopm_linked_list_destroy(lnk_lst);
}


void test_iterator_checking_next(){//här kommer alla iterator tests
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(1));
  ioopm_linked_list_append(lnk_lst, int_elem(2));
  ioopm_linked_list_append(lnk_lst, int_elem(3));

  ioopm_list_iterator_t *iter = ioopm_list_iterator(lnk_lst);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 1);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 2);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 3);

  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(lnk_lst);
}

void test_iterator_reset(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(1));
  ioopm_linked_list_append(lnk_lst, int_elem(2));

  ioopm_list_iterator_t *iter = ioopm_list_iterator(lnk_lst);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 1);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 2);

  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));

  ioopm_iterator_reset(iter);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 1);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 2);

  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(lnk_lst);
}

void test_iterator_empty_list()
{
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(lnk_lst);

  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(lnk_lst);
}

void test_current_iterator(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create(NULL);
  ioopm_linked_list_append(lnk_lst, int_elem(1));
  ioopm_linked_list_append(lnk_lst, int_elem(2));
  ioopm_list_iterator_t *iter = ioopm_list_iterator(lnk_lst);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_current(iter).i, 1);
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 1);

  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  CU_ASSERT_EQUAL(ioopm_iterator_next(iter).i, 2);

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(lnk_lst);
}


int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();
  
  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) { 
     // If the test suite could not be added, tear down CUnit and exit
    CU_cleanup_registry(); 
    return CU_get_error(); 
  }
  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information

  if (
    (CU_add_test(my_test_suite, "if the created ht is empty", test_lookup_empty) == NULL) ||
    (CU_add_test(my_test_suite, "creating and destroying a ht", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "update the value of an already existing key", test_update_existing_key) == NULL) ||
    (CU_add_test(my_test_suite, "testing multiple entrys in the same bucket", test_collision_same_bucket) == NULL) ||
    (CU_add_test(my_test_suite, "making sure the entry is inserted in the right way", test_insert_lookup) == NULL) ||
    (CU_add_test(my_test_suite, "checking so we dont get a key that does not exist", test_no_key_lookup) == NULL) ||
    (CU_add_test(my_test_suite, "testing if negative keys work", test_negative_keys) == NULL) ||
    (CU_add_test(my_test_suite, "checking if a key maped to NULL still returns true and the null value", test_null_value) == NULL) ||
    (CU_add_test(my_test_suite, "testing extreme keys", test_extreme_keys) == NULL) ||
    (CU_add_test(my_test_suite, "removing an entry that does not exist", test_remove_entry_fail) == NULL) ||
    (CU_add_test(my_test_suite, "removing an entry successfully", test_remove_entry_success) == NULL) ||
    (CU_add_test(my_test_suite, "Counts multiple entrys", test_count_entrys) == NULL) ||
    (CU_add_test(my_test_suite, "Counts entrys in a empty ht", test_count_entrys_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Counts one entry", test_count_one_entry) == NULL) ||
    (CU_add_test(my_test_suite, "first cheks if the ht-empty function works, then uses it to prove our clear function work aswell", test_hash_table_clear_and_empty) == NULL) ||
    (CU_add_test(my_test_suite, "kollar så att all keys funktionen får ut alla nycklar ur ht på korrekt sätt", test_all_keys) == NULL) ||
    (CU_add_test(my_test_suite, "kollar så att values funktionen får ut alla values ur ht på korrekt sätt", test_all_values) == NULL) ||
    (CU_add_test(my_test_suite, "kollar om sökt nyckel finns", test_if_key_exist) == NULL) ||
    (CU_add_test(my_test_suite, "kollar om sökt value finns", test_if_value_exist) == NULL) ||
    (CU_add_test(my_test_suite, "kollar om sökt predikat stämmer på någon entry", test_pred_any) == NULL) ||
    (CU_add_test(my_test_suite, "kollar om sökt predikat stämmer på alla entrys", test_pred_all) == NULL) ||
    (CU_add_test(my_test_suite, "applicerar en funktion på alla entrys i ht", test_apply_to_all) == NULL)
  ) 
  { 
    // If adding any of the tests fails, we tear down CUnit and exit
    CU_cleanup_registry(); 
    return CU_get_error(); 
  }

  CU_pSuite list_suite = CU_add_suite("Linked list suite", init_suite_list, clean_suite_list);
  if (!list_suite) { CU_cleanup_registry(); return CU_get_error(); }

  if (
    (CU_add_test(list_suite, "creating and destroying a linked list", test_list_create_destroy) == NULL) ||
    (CU_add_test(list_suite, "prepend inserts at head", test_prepend) == NULL) ||
    (CU_add_test(list_suite, "append + get", test_linked_list_append_and_get) == NULL) ||
    (CU_add_test(list_suite, "insert at index", test_insert_list_and_prepend) == NULL) ||
    (CU_add_test(list_suite, "remove at index", test_linked_list_remove) == NULL) ||
    (CU_add_test(list_suite, "clear empties list", test_linked_list_clear) == NULL) ||
    (CU_add_test(list_suite, "all predicate", test_linked_list_all) == NULL) ||
    (CU_add_test(list_suite, "any predicate", test_linked_list_any) == NULL) ||
    (CU_add_test(list_suite, "apply_to_all mutates values", test_linked_list_apply_to_all) == NULL) ||
    (CU_add_test(list_suite, "contains finds existing element", test_linked_list_contains) == NULL)
  ) { CU_cleanup_registry(); return CU_get_error(); }

  CU_pSuite iterator_suite = CU_add_suite("Iterator suite", init_suite_iterator, clean_suite_iterator);
  if (!iterator_suite) { CU_cleanup_registry(); return CU_get_error(); }

  if (
    (CU_add_test(iterator_suite, "Testing both our has_next and next functions", test_iterator_checking_next) == NULL) ||
    (CU_add_test(iterator_suite, "Testing our iterator reset function", test_iterator_reset) == NULL) ||
    (CU_add_test(iterator_suite, "Testing our iterator in an empty list", test_iterator_empty_list) == NULL) ||
    (CU_add_test(iterator_suite, "Testing our iterator current function", test_current_iterator) == NULL)
  ) { CU_cleanup_registry(); return CU_get_error(); }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);
 
  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
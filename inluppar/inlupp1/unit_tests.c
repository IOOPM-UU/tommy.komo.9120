#include <CUnit/Basic.h>
#include "hash_table.h"
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include "linked_list.h"
#include <string.h>

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

int init_suite_list(void) {
  return 0;
}

int clean_suite_list(void) {
  return 0;
}

void test_lookup_empty()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   
   for (int i = 0; i < 18; ++i) /// 18 is chosen due to our 17 pre-chosen buckets that can't be changed
     {
      char *v = NULL; 
      CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, i, &v));
      CU_ASSERT_PTR_NULL(v);
     }

   ioopm_hash_table_destroy(ht);
}



void test_create_destroy()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}



void test_update_existing_key(){
     // Testar att ändra värdet en redan existerande nyckel (använder värdet från förra testet)

    ioopm_hash_table_t *ht = ioopm_hash_table_create();

    ioopm_hash_table_insert(ht, 5, "test");
    ioopm_hash_table_insert(ht, 5, "rätt" );
    char *keyvalue = NULL;
    bool truefalse = ioopm_hash_table_lookup(ht, 5, &keyvalue);
    CU_ASSERT_TRUE(truefalse);
    CU_ASSERT_STRING_EQUAL(keyvalue, "rätt");
    ioopm_hash_table_destroy(ht);

}


void test_collision_same_bucket()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 5, "a");
  ioopm_hash_table_insert(ht, 22, "b");

  char *keyvalue = NULL;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, 22, &keyvalue));
  CU_ASSERT_STRING_EQUAL(keyvalue, "b");

  char *keyvalue2 = NULL;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, 5, &keyvalue2));
  CU_ASSERT_STRING_EQUAL(keyvalue2, "a");

  ioopm_hash_table_destroy(ht);
}


void test_insert_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 32, "hello");

  char *keyvalue = NULL;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, 32, &keyvalue));
  CU_ASSERT_STRING_EQUAL(keyvalue, "hello");

  ioopm_hash_table_destroy(ht);
}



void test_key_in_use()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 7, "test");
  ioopm_hash_table_insert(ht, 7, "correct");

  char *keyvalue = NULL;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, 7, &keyvalue));
  CU_ASSERT_STRING_EQUAL(keyvalue, "correct");

  ioopm_hash_table_destroy(ht);
}

void test_no_key_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 7, "test");

  char *keyvalue = NULL;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, 8, &keyvalue));

  ioopm_hash_table_destroy(ht);
}

void test_negative_keys()  // Kan komma att ändra senare vid implementaion
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  ioopm_hash_table_insert(ht, -7, "minusseven");

  char *keyvalue = NULL;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, -7, &keyvalue));
  CU_ASSERT_STRING_EQUAL(keyvalue, "minusseven");

  ioopm_hash_table_destroy(ht);
}

void test_null_value(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 1, NULL);

  char *keyvalue = "dummy";
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht,1, &keyvalue));
  CU_ASSERT_PTR_NULL(keyvalue);

  ioopm_hash_table_destroy(ht);
}

void test_extreme_keys(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  ioopm_hash_table_insert(ht, INT_MAX, "max");
  ioopm_hash_table_insert(ht, INT_MIN, "min");

  char *keyvalue = NULL;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, INT_MAX, &keyvalue));
  CU_ASSERT_STRING_EQUAL(keyvalue, "max");

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, INT_MIN, &keyvalue));
  CU_ASSERT_STRING_EQUAL(keyvalue, "min");

  ioopm_hash_table_destroy(ht);
}

void test_remove_entry_fail()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  bool truefalse = ioopm_hash_table_remove(ht, 32, NULL);
  CU_ASSERT_FALSE(truefalse);

  ioopm_hash_table_destroy(ht);
}

void test_remove_entry_success()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 32, "hello");
  
  char *keyvalue = NULL;
  bool truefalse = ioopm_hash_table_remove(ht, 32, &keyvalue);
  
  CU_ASSERT_TRUE(truefalse);
  CU_ASSERT_STRING_EQUAL(keyvalue, "hello");

  ioopm_hash_table_destroy(ht);
}

void test_count_entrys_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  int x = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(x, 0);

  ioopm_hash_table_destroy(ht);
}

void test_count_one_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  ioopm_hash_table_insert(ht, 32, "hello");
  int x = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(x, 1);

  ioopm_hash_table_destroy(ht);
}

void test_count_entrys()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  ioopm_hash_table_insert(ht, 5, "h");
  ioopm_hash_table_insert(ht, 0, "e");
  ioopm_hash_table_insert(ht, 30, "l");
  ioopm_hash_table_insert(ht, 17, "l");
  ioopm_hash_table_insert(ht, 6, "o");
 
  int x = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(x, 5);
  
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_clear_and_empty(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  ioopm_hash_table_insert(ht, 5, "h");
  ioopm_hash_table_insert(ht, 0, "e");
  ioopm_hash_table_insert(ht, 30, "l");
  ioopm_hash_table_insert(ht, 17, "l");
  ioopm_hash_table_insert(ht, 6, "o");

  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_clear(ht);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_all_keys(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  int keys[5] = {3, 10, 42, 0, 99};
  bool found[5] = {false};
  ioopm_hash_table_insert(ht, 3, "h");
  ioopm_hash_table_insert(ht, 10, "e");
  ioopm_hash_table_insert(ht, 42, "l");
  ioopm_hash_table_insert(ht, 0, "l");
  ioopm_hash_table_insert(ht, 99, "o");

  int *table_keys = ioopm_hash_table_keys(ht);
  for(int i = 0; i < 5 ; i++){ //loop för alla nycklar vi borde ha, i -> keys[i]
   
    bool match_found = false; //fick hjälp av chat att implementera
    
    for(int z = 0; z < 5 ; z++){ //loop för alla nycklar vi har
      
      if(keys[i] == table_keys[z])
      {
        found[i] = true;
        match_found = true;
        break;
      } 
    } if (match_found == false){
      CU_FAIL("Found a key that was never inserted!");
    }
  }
  for(int i = 0; i < 5 ; i++){ //alla värden ska vara true om funktionerna funkar som de ska
      CU_ASSERT_TRUE(found[i]);
    }
  free(table_keys);
  
  ioopm_hash_table_destroy(ht); 
}


void test_all_values()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  int keys[5] = {3, 10, 42, 0, 99};
  char *values[5] = {"three","ten","fortytwo","zero","ninetynine"};
  bool found[5] = {false, false, false, false, false};

  for(int i = 0; i<5 ; i++){
    ioopm_hash_table_insert(ht, keys[i], values[i]);  // lägg in korresponderande nyckel med värde 
  }
  
  char **table_values = ioopm_hash_table_values(ht);

  for (int i= 0; i < 5; i++){
    bool match_found = false;
    for (int j = 0; j < 5; ++j){
      if (values[i] == table_values[j]) {
        found[i] = true;
        match_found = true;
        break;
      }
    }
    if (!match_found){
      CU_FAIL("Found a value that was never inserted!");
    }
  }

  for(int i = 0; i < 5; i++){
    CU_ASSERT_TRUE(found[i]);
  } 

  free(table_values);
  
  ioopm_hash_table_destroy(ht);
}

void test_if_key_exist(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 5, "hej");
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, 5));
  ioopm_hash_table_insert(ht, 22, "då" );
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, 5));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, 22));

  ioopm_hash_table_destroy(ht); 
}

void test_if_value_exist(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 5, "hej");
  CU_ASSERT_TRUE(ioopm_hash_table_has_values(ht, "hej"));
  
  char *copy = strdup("hej");  
  CU_ASSERT_TRUE(ioopm_hash_table_has_values(ht, copy));
  free(copy);

  ioopm_hash_table_destroy(ht); 
}



bool value_is_hej(int key, char *value, void *x)
{
    return strcmp(value, "hej") == 0;
}

void test_pred_any(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 5, "hej");
  ioopm_hash_table_insert(ht, 22, "fel");

  bool result = ioopm_hash_table_any(ht, value_is_hej, NULL);
  CU_ASSERT_TRUE(result);
  
  ioopm_hash_table_clear(ht);
  
  ioopm_hash_table_insert(ht, 22, "fel");
  bool wrong_result = ioopm_hash_table_any(ht, value_is_hej, NULL);
  CU_ASSERT_FALSE(wrong_result);

  ioopm_hash_table_destroy(ht);
}

void test_pred_all(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 5, "hej");
  ioopm_hash_table_insert(ht, 22, "hej");
  ioopm_hash_table_insert(ht, 25, "hej");
  ioopm_hash_table_insert(ht, 0, "hej");
  ioopm_hash_table_insert(ht, 16, "hej");

  bool result = ioopm_hash_table_all(ht, value_is_hej, NULL);
  CU_ASSERT_TRUE(result);
  
  ioopm_hash_table_clear(ht);
  
  ioopm_hash_table_insert(ht, 22, "hej");
  ioopm_hash_table_insert(ht, 25, "hej");
  ioopm_hash_table_insert(ht, 0, "hej");
  ioopm_hash_table_insert(ht, 16, "hej");
  ioopm_hash_table_insert(ht, 22, "fel");
  
  bool wrong_result = ioopm_hash_table_all(ht, value_is_hej, NULL);
  CU_ASSERT_FALSE(wrong_result);

  ioopm_hash_table_destroy(ht);
}

void apply_string_to_all(int key_ignore, char **value, void *extra)
{
  char *str = extra;
  *value = str;
}


void test_apply_to_all()
{
  char *str = "paint";
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
    
  ioopm_hash_table_insert(ht, 1, "green");
  ioopm_hash_table_insert(ht, 2, "blue");
  ioopm_hash_table_insert(ht, 3, "red");

  ioopm_hash_table_apply_to_all(ht, apply_string_to_all, str);

  CU_ASSERT_TRUE(ioopm_hash_table_has_values(ht, "paint"));
  CU_ASSERT_FALSE(ioopm_hash_table_has_values(ht, "green"));
  CU_ASSERT_FALSE(ioopm_hash_table_has_values(ht, "blue"));
  CU_ASSERT_FALSE(ioopm_hash_table_has_values(ht, "red"));

  ioopm_hash_table_destroy(ht);
}

void test_list_create_destroy(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();

  CU_ASSERT_PTR_NOT_NULL(list);

  ioopm_linked_list_destroy(list);
}

void test_prepend(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();

  ioopm_linked_list_prepend(list, 3);

  CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 1);
  CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0), 3);
  
  ioopm_linked_list_destroy(list);
}


void test_linked_list_append_and_get(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create();
  ioopm_linked_list_append(lnk_lst, 4);
  ioopm_linked_list_append(lnk_lst, 5);
  
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 1), 5);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_insert_list_and_prepend(){
   ioopm_list_t *lnk_lst = ioopm_linked_list_create();

   ioopm_linked_list_insert(lnk_lst, 0, 7);
   CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 0), 7);

   ioopm_linked_list_append(lnk_lst, 4);
   ioopm_linked_list_insert(lnk_lst, 1, 8);
   CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 1), 8);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_remove(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create();
  ioopm_linked_list_append(lnk_lst, 3);

  ioopm_linked_list_remove(lnk_lst, 0);

  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lnk_lst));
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lnk_lst), 0);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_clear(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create();
  ioopm_linked_list_append(lnk_lst, 1);
  ioopm_linked_list_append(lnk_lst, 2);
  ioopm_linked_list_append(lnk_lst, 3);

  ioopm_linked_list_clear(lnk_lst);

  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lnk_lst));
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lnk_lst), 0);

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_contains(){
  ioopm_list_t *list = ioopm_linked_list_create();

  ioopm_linked_list_append(list, 10);
  ioopm_linked_list_append(list, 20);
  ioopm_linked_list_append(list, 30);

  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 20));
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list, 99));
  
  ioopm_linked_list_destroy(list);
}

bool is_odd(int value, void *extra) {
    (void) extra; // används inte
    return value % 2 == 1;
}

void test_linked_list_all()
{
  ioopm_list_t *lnk_lst = ioopm_linked_list_create();
  ioopm_linked_list_append(lnk_lst, 1);
  ioopm_linked_list_append(lnk_lst, 3);
  ioopm_linked_list_append(lnk_lst, 5);

  CU_ASSERT_TRUE(ioopm_linked_list_all(lnk_lst, is_odd, NULL));

  ioopm_linked_list_destroy(lnk_lst);
}

void test_linked_list_any(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create();
  ioopm_linked_list_append(lnk_lst, 2);
  ioopm_linked_list_append(lnk_lst, 4);

  
  CU_ASSERT_FALSE(ioopm_linked_list_any(lnk_lst, is_odd, NULL));

  ioopm_linked_list_append(lnk_lst, 5);
  CU_ASSERT_TRUE(ioopm_linked_list_any(lnk_lst, is_odd, NULL));

  ioopm_linked_list_destroy(lnk_lst);
}

static void add_n(int *value, void *extra) {
    int *n = extra;
    *value += *n;
}

void test_linked_list_apply_to_all(){
  ioopm_list_t *lnk_lst = ioopm_linked_list_create();

  ioopm_linked_list_append(lnk_lst, 1);
  ioopm_linked_list_append(lnk_lst, 2);
  ioopm_linked_list_append(lnk_lst, 3);

  int nmr = 1;
  ioopm_linked_list_apply_to_all(lnk_lst, add_n, &nmr);

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 0), 2);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 1), 3);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lnk_lst, 2), 4);

  ioopm_linked_list_destroy(lnk_lst);
}

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

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
    (CU_add_test(my_test_suite, "applicerar en funktion på alla entrys i ht", test_apply_to_all) == NULL) ||

    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }


  CU_pSuite list_suite = CU_add_suite("Linked list suite", init_suite_list, clean_suite_list); 
  if (list_suite == NULL) {                                                                     
    CU_cleanup_registry();                                                                  
    return CU_get_error();                                                                    
  }   

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
  )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();

                                                                  
}
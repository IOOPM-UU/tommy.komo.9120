#include <CUnit/Basic.h>
#include <CUnit/Basic.h>
#include "hash_table.h"



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

// These are example test functions. You should replace them with
// functions of your own.
void test_create_destroy()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
   //CU_ASSERT_PTR_NULL(ht);
}

void test_insert_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 32, "hello");

  char *value = ioopm_hash_table_lookup(ht, 32);

  CU_ASSERT_STRING_EQUAL(value, "hello");
  ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *value = ioopm_hash_table_lookup(ht, 15);

  CU_ASSERT_PTR_NULL(value);

  ioopm_hash_table_insert(ht, 15, "hello");
  char *value2 = ioopm_hash_table_lookup(ht, 15);

  CU_ASSERT_STRING_EQUAL(value2, "hello");
  ioopm_hash_table_destroy(ht);
}

void test_key_in_use()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 7, "test");
  ioopm_hash_table_insert(ht, 7, "correct");

  char *value = ioopm_hash_table_lookup(ht, 7);
  CU_ASSERT_STRING_EQUAL(value, "correct");
  ioopm_hash_table_destroy(ht);
}

void test_no_key_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 7, "test");

  char *value = ioopm_hash_table_lookup(ht, 2);
  CU_ASSERT_PTR_NULL(value);
  ioopm_hash_table_destroy(ht);
}

void test_negative_keys()  // Kan komma att ändra senare vid implementaion
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 7, "test");

  char *value = ioopm_hash_table_lookup(ht, -7);
  CU_ASSERT_STRING_EQUAL(value, "test");
  ioopm_hash_table_destroy(ht);
  
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
    (CU_add_test(my_test_suite, "create and destroy hashtable", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "insert and lookup an entry", test_insert_lookup) == NULL) || 
    (CU_add_test(my_test_suite, "insert and lookup an entry", test_insert_once) == NULL) ||
    (CU_add_test(my_test_suite, "insert and lookup an entry", test_key_in_use) == NULL) ||
    (CU_add_test(my_test_suite, "insert and lookup an entry", test_negative_keys) == NULL) ||
    (CU_add_test(my_test_suite, "insert and lookup an entry", test_no_key_lookup) == NULL) ||
    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
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
#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "backend.h"
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

int init_suite(void)  { return 0; }
int clean_suite(void) { return 0; }

/* ========= Hjälpfunktioner för tester ========= */

/// Summera total mängd av en merch i lagret (över alla hyllor)
static int total_quantity_for_merch(db_t *db, const char *name)
{
    merch_t *m = merch_lookup(db, (char *)name);
    if (!m) return 0;

    int total = 0;
    size_t n = ioopm_linked_list_size(m->locations);
    for (size_t i = 0; i < n; ++i)
    {
        stock_entry_t *entry = ioopm_linked_list_get(m->locations, i).p;
        total += entry->quantity;
    }
    return total;
}

/// Kolla om en merch har en hylla med visst namn och viss kvantitet
static bool merch_has_location(merch_t *m, const char *loc, int qty)
{
    size_t n = ioopm_linked_list_size(m->locations);
    bool found = false;

    for (size_t i = 0; i < n; ++i)
    {
        stock_entry_t *entry = ioopm_linked_list_get(m->locations, i).p;
        if (strcmp(entry->location, loc) == 0)
        {
            CU_ASSERT_EQUAL(entry->quantity, qty);
            found = true;
        }
    }
    return found;
}

/* ========= Tester: grundfunktioner / merch ========= */

void test_db_create_and_destroy(void)
{
    db_t *db = db_create();
    CU_ASSERT_PTR_NOT_NULL(db);
    CU_ASSERT_PTR_NOT_NULL(db->name_to_merch);
    CU_ASSERT_PTR_NOT_NULL(db->loc_to_name);
    CU_ASSERT_PTR_NOT_NULL(db->carts);
    CU_ASSERT_EQUAL(db->next_cart_id, 1);

    db_destroy(db);
}

void test_add_merch_and_lookup(void)
{
    db_t *db = db_create();

    bool ok = add_merch(db, "apple", "red apple", 10);
    CU_ASSERT_TRUE(ok);

    merch_t *m = merch_lookup(db, "apple");
    CU_ASSERT_PTR_NOT_NULL(m);
    CU_ASSERT_STRING_EQUAL(m->name, "apple");
    CU_ASSERT_STRING_EQUAL(m->desc, "red apple");
    CU_ASSERT_EQUAL(m->price, 10);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(m->locations), (size_t)0);

    db_destroy(db);
}

void test_add_merch_duplicate_name_fails(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple", "A", 10));
    CU_ASSERT_FALSE(add_merch(db, "apple", "B", 20));  // samma namn -> ska misslyckas

    merch_t *m = merch_lookup(db, "apple");
    CU_ASSERT_PTR_NOT_NULL(m);
    // borde fortfarande vara första värdena
    CU_ASSERT_STRING_EQUAL(m->desc, "A");
    CU_ASSERT_EQUAL(m->price, 10);

    db_destroy(db);
}

void test_merch_lookup_nonexistent(void)
{
    db_t *db = db_create();

    CU_ASSERT_PTR_NULL(merch_lookup(db, "does_not_exist"));

    db_destroy(db);
}

/* ========= Tester: is_valid_shelf & replenish ========= */

void test_is_valid_shelf(void)
{
    CU_ASSERT_TRUE(is_valid_shelf("A1"));
    CU_ASSERT_TRUE(is_valid_shelf("Z99"));
    CU_ASSERT_TRUE(is_valid_shelf("B123"));

    CU_ASSERT_FALSE(is_valid_shelf(NULL));
    CU_ASSERT_FALSE(is_valid_shelf(""));
    CU_ASSERT_FALSE(is_valid_shelf("A"));     // för kort
    CU_ASSERT_FALSE(is_valid_shelf("a1"));    // första är inte versal
    CU_ASSERT_FALSE(is_valid_shelf("1A"));    // första är siffra
    CU_ASSERT_FALSE(is_valid_shelf("A1B"));   // sista är inte siffra
}

void test_replenish_adds_stock_and_loc_mapping(void)
{
    db_t *db = db_create();
    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));

    bool ok = replenish(db, "A1", "apple", 5);
    CU_ASSERT_TRUE(ok);

    merch_t *m = merch_lookup(db, "apple");
    CU_ASSERT_PTR_NOT_NULL(m);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(m->locations), (size_t)1);

    stock_entry_t *entry = ioopm_linked_list_get(m->locations, 0).p;
    CU_ASSERT_STRING_EQUAL(entry->location, "A1");
    CU_ASSERT_EQUAL(entry->quantity, 5);

    elem_t v = {0};
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->loc_to_name, ptr_elem("A1"), &v));
    CU_ASSERT_STRING_EQUAL((char *)v.p, "apple");

    db_destroy(db);
}

void test_replenish_multiple_locations(void)
{
    db_t *db = db_create();
    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));

    CU_ASSERT_TRUE(replenish(db, "A1", "apple", 5));
    CU_ASSERT_TRUE(replenish(db, "B2", "apple", 3));

    merch_t *m = merch_lookup(db, "apple");
    CU_ASSERT_PTR_NOT_NULL(m);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(m->locations), (size_t)2);

    bool found_A1 = merch_has_location(m, "A1", 5);
    bool found_B2 = merch_has_location(m, "B2", 3);
    CU_ASSERT_TRUE(found_A1);
    CU_ASSERT_TRUE(found_B2);

    db_destroy(db);
}

void test_replenish_nonexistent_merch_fails(void)
{
    db_t *db = db_create();

    CU_ASSERT_FALSE(replenish(db, "A1", "nope", 5));

    db_destroy(db);
}

/* ========= Tester: edit_merch & remove_merch ========= */

void test_edit_merch_change_name_desc_price_and_locations_mapping(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "milk", "fresh", 20));
    CU_ASSERT_TRUE(replenish(db, "A1", "milk", 5));
    CU_ASSERT_TRUE(replenish(db, "B2", "milk", 2));

    bool ok = edit_merch(db, "milk", "oatmilk", "oat-based", 25);
    CU_ASSERT_TRUE(ok);

    // gamla namnet ska inte finnas
    CU_ASSERT_PTR_NULL(merch_lookup(db, "milk"));

    // nya namnet ska finnas
    merch_t *m = merch_lookup(db, "oatmilk");
    CU_ASSERT_PTR_NOT_NULL(m);
    CU_ASSERT_STRING_EQUAL(m->name, "oatmilk");
    CU_ASSERT_STRING_EQUAL(m->desc, "oat-based");
    CU_ASSERT_EQUAL(m->price, 25);

    // location -> name mapping ska uppdateras
    elem_t v = {0};
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->loc_to_name, ptr_elem("A1"), &v));
    CU_ASSERT_STRING_EQUAL((char *)v.p, "oatmilk");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->loc_to_name, ptr_elem("B2"), &v));
    CU_ASSERT_STRING_EQUAL((char *)v.p, "oatmilk");

    db_destroy(db);
}

void test_edit_merch_name_conflict_fails(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "a", "desc a", 10));
    CU_ASSERT_TRUE(add_merch(db, "b", "desc b", 20));

    // försök döpa om "a" till "b" -> ska misslyckas
    CU_ASSERT_FALSE(edit_merch(db, "a", "b", "new desc", 30));

    merch_t *ma = merch_lookup(db, "a");
    merch_t *mb = merch_lookup(db, "b");
    CU_ASSERT_PTR_NOT_NULL(ma);
    CU_ASSERT_PTR_NOT_NULL(mb);
    CU_ASSERT_STRING_EQUAL(ma->name, "a");
    CU_ASSERT_STRING_EQUAL(mb->name, "b");

    db_destroy(db);
}

void test_remove_merch_removes_from_both_maps(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "chips", "salty", 15));
    CU_ASSERT_TRUE(replenish(db, "C3", "chips", 5));

    CU_ASSERT_TRUE(remove_merch(db, "chips"));

    CU_ASSERT_PTR_NULL(merch_lookup(db, "chips"));

    // hyllan ska inte längre vara mappad i loc_to_name
    elem_t v = {0};
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(db->loc_to_name, ptr_elem("C3"), &v));

    db_destroy(db);
}

void test_remove_merch_nonexistent_fails(void)
{
    db_t *db = db_create();

    CU_ASSERT_FALSE(remove_merch(db, "does_not_exist"));

    db_destroy(db);
}

/* ========= Tester: kundvagnar ========= */

void test_create_and_remove_cart(void)
{
    db_t *db = db_create();

    CU_ASSERT_EQUAL(ioopm_linked_list_size(db->carts), (size_t)0);

    cart_t *c1 = create_cart(db);
    cart_t *c2 = create_cart(db);

    CU_ASSERT_PTR_NOT_NULL(c1);
    CU_ASSERT_PTR_NOT_NULL(c2);
    CU_ASSERT_EQUAL(c1->index + 1, c2->index);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(db->carts), (size_t)2);

    CU_ASSERT_PTR_NOT_NULL(cart_lookup(db, c1->index));
    CU_ASSERT_TRUE(remove_cart(db, c1->index));
    CU_ASSERT_PTR_NULL(cart_lookup(db, c1->index));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(db->carts), (size_t)1);

    CU_ASSERT_FALSE(remove_cart(db, 999)); // finns inte

    db_destroy(db);
}

void test_cart_lookup_missing_returns_null(void)
{
    db_t *db = db_create();

    CU_ASSERT_PTR_NULL(cart_lookup(db, 1));

    db_destroy(db);
}

/* ========= Tester: add_to_cart ========= */

void test_add_to_cart_invalid_cart_or_merch(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));
    CU_ASSERT_TRUE(replenish(db, "A1", "apple", 5));

    cart_t *c = create_cart(db);
    int cid = c->index;

    // ogiltig cart
    CU_ASSERT_FALSE(add_to_cart(db, cid + 100, "apple", 1));

    // ogiltig merch
    CU_ASSERT_FALSE(add_to_cart(db, cid, "does_not_exist", 1));

    db_destroy(db);
}

void test_add_to_cart_not_enough_stock_fails(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));
    CU_ASSERT_TRUE(replenish(db, "A1", "apple", 3));   // totalt 3 på lager

    cart_t *c1 = create_cart(db);
    cart_t *c2 = create_cart(db);

    CU_ASSERT_TRUE(add_to_cart(db, c1->index, "apple", 2));  // reserverar 2
    CU_ASSERT_FALSE(add_to_cart(db, c2->index, "apple", 2)); // bara 1 kvar -> ska misslyckas

    db_destroy(db);
}

void test_add_to_cart_accumulates_in_same_cart(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));
    CU_ASSERT_TRUE(replenish(db, "A1", "apple", 10)); // gott om

    cart_t *c = create_cart(db);
    int cid = c->index;

    CU_ASSERT_TRUE(add_to_cart(db, cid, "apple", 3));
    CU_ASSERT_TRUE(add_to_cart(db, cid, "apple", 2));

    cart_t *cart = cart_lookup(db, cid);
    CU_ASSERT_PTR_NOT_NULL(cart);

    elem_t val = {0};
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(cart->items, ptr_elem("apple"), &val));
    CU_ASSERT_EQUAL(val.i, 5);

    db_destroy(db);
}

/* ========= Tester: remove_from_cart ========= */

void test_remove_from_cart_error_cases(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));
    CU_ASSERT_TRUE(replenish(db, "A1", "apple", 5));

    cart_t *c = create_cart(db);
    int cid = c->index;

    CU_ASSERT_TRUE(add_to_cart(db, cid, "apple", 2));

    // Kartan finns inte
    CU_ASSERT_FALSE(remove_from_cart(db, cid + 100, "apple", 1));

    // Merch finns inte i cart
    CU_ASSERT_FALSE(remove_from_cart(db, cid, "banana", 1));

    // För mycket
    CU_ASSERT_FALSE(remove_from_cart(db, cid, "apple", 3));

    db_destroy(db);
}

void test_remove_from_cart_success_partial_and_full(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));
    CU_ASSERT_TRUE(replenish(db, "A1", "apple", 10));

    cart_t *c = create_cart(db);
    int cid = c->index;

    CU_ASSERT_TRUE(add_to_cart(db, cid, "apple", 5));

    // ta bort 2 -> 3 kvar
    CU_ASSERT_TRUE(remove_from_cart(db, cid, "apple", 2));
    cart_t *cart = cart_lookup(db, cid);
    elem_t val = {0};
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(cart->items, ptr_elem("apple"), &val));
    CU_ASSERT_EQUAL(val.i, 3);

    // ta bort resterande 3 -> nyckeln ska bort
    CU_ASSERT_TRUE(remove_from_cart(db, cid, "apple", 3));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(cart->items, ptr_elem("apple")));

    db_destroy(db);
}

/* ========= Tester: calc_cost ========= */

void test_calc_cost_basic(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple",  "red", 10));
    CU_ASSERT_TRUE(add_merch(db, "banana", "yellow", 7));

    CU_ASSERT_TRUE(replenish(db, "A1", "apple",  10));
    CU_ASSERT_TRUE(replenish(db, "B2", "banana", 10));

    cart_t *c = create_cart(db);
    int cid = c->index;

    CU_ASSERT_TRUE(add_to_cart(db, cid, "apple",  3)); // 3 * 10 = 30
    CU_ASSERT_TRUE(add_to_cart(db, cid, "banana", 2)); // 2 *  7 = 14

    int cost = calc_cost(db, cid);
    CU_ASSERT_EQUAL(cost, 44);

    db_destroy(db);
}

void test_calc_cost_merch_removed_from_db(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple",  "red", 10));
    CU_ASSERT_TRUE(add_merch(db, "banana", "yellow", 7));

    CU_ASSERT_TRUE(replenish(db, "A1", "apple",  10));
    CU_ASSERT_TRUE(replenish(db, "B2", "banana", 10));

    cart_t *c = create_cart(db);
    int cid = c->index;

    CU_ASSERT_TRUE(add_to_cart(db, cid, "apple",  3));
    CU_ASSERT_TRUE(add_to_cart(db, cid, "banana", 2));

    // Ta bort bananen ur DB efter att den lagts i varukorgen
    CU_ASSERT_TRUE(remove_merch(db, "banana"));

    // Kostnaden ska då bara vara äpplen (3 * 10)
    int cost = calc_cost(db, cid);
    CU_ASSERT_EQUAL(cost, 30);

    db_destroy(db);
}

void test_calc_cost_invalid_cart_returns_zero(void)
{
    db_t *db = db_create();

    int cost = calc_cost(db, 999);
    CU_ASSERT_EQUAL(cost, 0);

    db_destroy(db);
}

/* ========= Tester: checkout ========= */

void test_checkout_reduces_stock_and_removes_cart(void)
{
    db_t *db = db_create();

    CU_ASSERT_TRUE(add_merch(db, "apple", "red", 10));
    CU_ASSERT_TRUE(replenish(db, "A1", "apple", 2));
    CU_ASSERT_TRUE(replenish(db, "B2", "apple", 3)); // totalt 5

    cart_t *c = create_cart(db);
    int cid = c->index;

    CU_ASSERT_TRUE(add_to_cart(db, cid, "apple", 4)); // lämnar 1 kvar

    int cost_before = calc_cost(db, cid);
    CU_ASSERT_EQUAL(cost_before, 4 * 10);

    CU_ASSERT_TRUE(checkout(db, cid));

    // carten ska vara borttagen
    CU_ASSERT_PTR_NULL(cart_lookup(db, cid));

    // total mängd i lagret ska vara 1
    int total = total_quantity_for_merch(db, "apple");
    CU_ASSERT_EQUAL(total, 1);

    // A1 borde vara tömd och därmed borttagen ur loc_to_name
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(db->loc_to_name, ptr_elem("A1")));

    // B2 bör fortfarande finnas och vara kopplad till "apple"
    elem_t v = {0};
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->loc_to_name, ptr_elem("B2"), &v));
    CU_ASSERT_STRING_EQUAL((char *)v.p, "apple");

    db_destroy(db);
}

void test_checkout_invalid_cart_fails(void)
{
    db_t *db = db_create();

    CU_ASSERT_FALSE(checkout(db, 123));

    db_destroy(db);
}

/* ========= main ========= */

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Shopping / backend suite", init_suite, clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Lägg till tester här */
    if (
        (CU_add_test(suite, "db_create & db_destroy",                test_db_create_and_destroy) == NULL) ||
        (CU_add_test(suite, "add_merch + merch_lookup",              test_add_merch_and_lookup) == NULL) ||
        (CU_add_test(suite, "add_merch duplicate name fails",        test_add_merch_duplicate_name_fails) == NULL) ||
        (CU_add_test(suite, "merch_lookup på icke-existerande",      test_merch_lookup_nonexistent) == NULL) ||
        (CU_add_test(suite, "is_valid_shelf",                        test_is_valid_shelf) == NULL) ||
        (CU_add_test(suite, "replenish lägger till lager & loc-map", test_replenish_adds_stock_and_loc_mapping) == NULL) ||
        (CU_add_test(suite, "replenish flera hyllor",                test_replenish_multiple_locations) == NULL) ||
        (CU_add_test(suite, "replenish på icke-existerande merch",   test_replenish_nonexistent_merch_fails) == NULL) ||
        (CU_add_test(suite, "edit_merch uppdaterar allt korrekt",    test_edit_merch_change_name_desc_price_and_locations_mapping) == NULL) ||
        (CU_add_test(suite, "edit_merch namnkonflikt",               test_edit_merch_name_conflict_fails) == NULL) ||
        (CU_add_test(suite, "remove_merch rensar mappar",            test_remove_merch_removes_from_both_maps) == NULL) ||
        (CU_add_test(suite, "remove_merch på saknad vara",           test_remove_merch_nonexistent_fails) == NULL) ||
        (CU_add_test(suite, "create_cart / remove_cart / lookup",    test_create_and_remove_cart) == NULL) ||
        (CU_add_test(suite, "cart_lookup saknad",                    test_cart_lookup_missing_returns_null) == NULL) ||
        (CU_add_test(suite, "add_to_cart ogiltig cart/merch",        test_add_to_cart_invalid_cart_or_merch) == NULL) ||
        (CU_add_test(suite, "add_to_cart otillräckligt lager",       test_add_to_cart_not_enough_stock_fails) == NULL) ||
        (CU_add_test(suite, "add_to_cart summerar i samma cart",     test_add_to_cart_accumulates_in_same_cart) == NULL) ||
        (CU_add_test(suite, "remove_from_cart fel-fall",             test_remove_from_cart_error_cases) == NULL) ||
        (CU_add_test(suite, "remove_from_cart delvis & helt",        test_remove_from_cart_success_partial_and_full) == NULL) ||
        (CU_add_test(suite, "calc_cost grundfall",                   test_calc_cost_basic) == NULL) ||
        (CU_add_test(suite, "calc_cost när merch tas bort ur DB",    test_calc_cost_merch_removed_from_db) == NULL) ||
        (CU_add_test(suite, "calc_cost ogiltig cart",                test_calc_cost_invalid_cart_returns_zero) == NULL) ||
        (CU_add_test(suite, "checkout minskar lager & tar bort cart",test_checkout_reduces_stock_and_removes_cart) == NULL) ||
        (CU_add_test(suite, "checkout ogiltig cart",                 test_checkout_invalid_cart_fails) == NULL)
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}

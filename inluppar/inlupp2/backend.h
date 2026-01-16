#pragma once

#include "common.h"
#include "hash_table.h"
#include "linked_list.h"
#include "utils.h"

/**
 * @file backend.h
 * @author Tommy Komo & Matteo Pasquariello 
 * @date  15 November 2025
 * @brief Backend module for managing merchandise, stock entries, shelves, and carts.
 *
 * This header defines the core data structures used by the store system and
 * declares all backend operations for manipulating the store database.
 *
 * Higher-level modules such as the UI interact with the backend through
 * these functions. All memory associated with merchandise, carts, and stock
 * is owned by the database instance and is released by `db_destroy()`
 *
 */


typedef struct database db_t;
typedef struct stock_entry stock_entry_t;
typedef struct merch merch_t;
typedef struct cart cart_t;


struct merch
{
    char *name;                 
    char *desc;             
    int price;                
    ioopm_list_t *locations;   
};


struct stock_entry
{
    char *location; 
    int quantity;  
};


struct cart
{
    int index;                    
    ioopm_hash_table_t *items;      
};


struct database
{
    ioopm_hash_table_t *name_to_merch; 
    ioopm_hash_table_t *loc_to_name;  
    ioopm_list_t *carts;           
    int next_cart_id;          
};


/// @brief Create a new empty database
/// @return A newly allocated db_t
db_t *db_create(void);

/// @brief Destroy the database and free all internal objects
/// @param db the database to destroy
void db_destroy(db_t *db);

/// @brief Add a new merchandise item
/// @param db the database
/// @param name unique merch name
/// @param desc description text
/// @param price integer price
/// @return true on success, false if duplicate name
bool add_merch(db_t *db, char *name, char *desc, int price);

/// @brief Remove an existing merchandise item
/// @param db the database
/// @param name merch to remove
/// @return true if removed, false if not found
bool remove_merch(db_t *db, char *name);

/// @brief Edit an existing merchandise item
/// @param db the database
/// @param name current merch name
/// @param new_name updated name (may be identical to old)
/// @param new_desc updated description
/// @param new_price updated price
/// @return true on success, false if merch missing or name conflict
bool edit_merch(db_t *db, char *name, char *new_name, char *new_desc, int new_price);

/// @brief Add stock for a merch at a shelf location
/// @param db the database
/// @param loc shelf location
/// @param name merch name
/// @param quantity amount to add
/// @return true if successful, false if shelf invalid or merch missing
bool replenish(db_t *db, char *loc, char *name, int quantity);

/// @brief Lookup a merch by name
/// @param db the database
/// @param merch name to search for
/// @return merch_t * or NULL if not found
merch_t *merch_lookup(db_t *db, char *merch);

/// @brief Create a new empty shopping cart
/// @param db the database
/// @return the newly created cart
cart_t *create_cart(db_t *db);

/// @brief Check if a shelf name is valid (e.g., "A12")
/// @param shelf the shelf string
/// @return true if valid
bool is_valid_shelf(char *shelf);

/// @brief Remove an existing cart
/// @param db the database
/// @param cart_index ID of cart to remove
/// @return true if the cart existed and was removed
bool remove_cart(db_t *db, int cart_index);

/// @brief Lookup a cart by ID
/// @param db the database
/// @param cart_id cart identifier
/// @return the cart or NULL
cart_t *cart_lookup(db_t *db, int cart_id);

/// @brief Add quantity of a merch to a cart
/// @param db the database
/// @param cart_id target cart
/// @param merch merch name
/// @param quantity amount to add
/// @return true on success, false if insufficient stock or invalid arguments
bool add_to_cart(db_t *db, int cart_id, char *merch, int quantity);

/// @brief Remove quantity of a merch from a cart
/// @param db the database
/// @param cart_id target cart
/// @param merch merch name
/// @param quantity amount to remove
/// @return true on success, false on invalid cart/item/amount
bool remove_from_cart(db_t *db, int cart_id, char *merch, int quantity);

/// @brief Calculate total cost of a cart
/// @param db the database
/// @param cart_id cart to evaluate
/// @return total cost, or 0 if invalid cart
int calc_cost(db_t *db, int cart_id);

/// @brief Perform checkout: reduce stock and delete the cart
/// @param db the database
/// @param cart_id cart to checkout
/// @return true if successful, false if cart missing
bool checkout(db_t *db, int cart_id);
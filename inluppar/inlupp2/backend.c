#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"
#include "utils.h"



typedef struct database db_t;
typedef struct stock_entry stock_entry_t;
typedef struct merch merch_t;
typedef struct cart cart_t;

struct merch 
{
    char *name;        
    char *desc;
    int price;
    ioopm_list_t *locations;   // ska innehålla locations där merch finns
};

struct database 
{
    ioopm_hash_table_t *name_to_merch;
    ioopm_hash_table_t *loc_to_name;
    ioopm_list_t *carts; // listan med carts
    int next_cart_id; //denna behövs eftersom det i uppgiften står att "Shopping carts are identified by a monotonically increasing number, i.e., the number of the i’th shopping cart created is i, regardless of how many shopping carts have been removed." så någonstans måste detta nummret bara finnas
};

struct stock_entry {
    char *location;
    int quantity;
};

struct cart
{
    int index;
    ioopm_hash_table_t *items;
};


db_t *db_create(void)
{
    db_t *db = calloc(1, sizeof(db_t));
    db->name_to_merch = ioopm_hash_table_create(string_knr_hash, string_key_eq, NULL);
    db->loc_to_name = ioopm_hash_table_create(string_knr_hash, string_key_eq, NULL);
    db->carts = ioopm_linked_list_create(NULL);
    db->next_cart_id = 1;

    return db;
}

void db_destroy(db_t *db)
{
    if (!db) return;

    /* -------------------------
       1. Free all merchandise
       ------------------------- */

    // Get a list of all merch pointers stored in name_to_merch
    ioopm_list_t *merch_values = ioopm_hash_table_values(db->name_to_merch);
    size_t merch_count = ioopm_linked_list_size(merch_values);

    for (size_t i = 0; i < merch_count; i++)
    {
        elem_t elem = ioopm_linked_list_get(merch_values, i);
        merch_t *m = elem.p;
        if (!m) continue;

        // Free all stock entries for this merch
        if (m->locations)
        {
            size_t loc_count = ioopm_linked_list_size(m->locations);
            for (size_t j = 0; j < loc_count; j++)
            {
                elem_t e = ioopm_linked_list_get(m->locations, j);
                stock_entry_t *entry = e.p;
                if (!entry) continue;

                // entry->location was allocated with strdup in replenish()
                free(entry->location);
                free(entry);
            }

            // Destroy the locations list itself
            ioopm_linked_list_destroy(m->locations);
        }

        // Free merch fields
        free(m->desc);
        free(m->name);
        free(m);
    }

    // Destroy the temporary list of merch values
    ioopm_linked_list_destroy(merch_values);

    /* --------------------------------------
       2. Free all carts and their contents
       -------------------------------------- */

    if (db->carts)
    {
        size_t cart_count = ioopm_linked_list_size(db->carts);

        for (size_t i = 0; i < cart_count; i++)
        {
            elem_t elem = ioopm_linked_list_get(db->carts, i);
            cart_t *cart = elem.p;
            if (!cart) continue;

            if (cart->items)
            {
                ioopm_hash_table_destroy(cart->items);
            }

            // Finally free the cart struct
            free(cart);
        }

        // Destroy the list of carts (frees only the link nodes)
        ioopm_linked_list_destroy(db->carts);
    }

    /* -----------------------------------
       3. Destroy the hash tables & db
       ----------------------------------- */

    ioopm_hash_table_destroy(db->name_to_merch);
    ioopm_hash_table_destroy(db->loc_to_name);

    free(db);
}



bool is_valid_shelf(char *shelf)
{
    if (shelf == NULL || strlen(shelf) < 2)
    {
        return false;
    }

    if (!isupper(shelf[0]))
    {
        return false;
    }

    for (size_t i = 1; i < strlen(shelf); i++)
    {
        if (!isdigit(shelf[i]))
        {
            return false;
        }
    }
    return true;
}


bool add_merch(db_t *db, char *name, char *desc, int price){
    
    if(ioopm_hash_table_lookup(db->name_to_merch, ptr_elem(name), NULL)){return false;}
    
    merch_t *new_merch = calloc(1, sizeof(merch_t));
    new_merch->name = strdup(name);
    new_merch->desc = strdup(desc);
    new_merch->price = price;
    new_merch->locations = ioopm_linked_list_create(NULL);  // KAN BEHÖVA ÄNDRAS SENARE

    ioopm_hash_table_insert(db->name_to_merch, ptr_elem(new_merch->name), ptr_elem(new_merch));
    

    return true;

}

bool remove_merch(db_t *db, char *name){
    elem_t merch;
    bool lookup = ioopm_hash_table_lookup(db->name_to_merch, ptr_elem(name), &merch);
    if (!lookup){
        return false;
    }
    
    merch_t *m = merch.p;
    
    size_t size = ioopm_linked_list_size(m->locations); 
    for(size_t i = 0; i < size; i++){
        stock_entry_t *entry = ioopm_linked_list_get(m->locations, i).p;
        ioopm_hash_table_remove(db->loc_to_name, ptr_elem(entry->location), NULL); // nu har vi tagit bort från hashtable
    }

    ioopm_hash_table_remove(db->name_to_merch, ptr_elem(name), NULL);

    
    for(size_t i = 0; i < size; i++){
        stock_entry_t *entry = ioopm_linked_list_get(m->locations, i).p;
        free(entry->location);
        free(entry);
    }

    ioopm_linked_list_destroy(m->locations);
    free(m->desc);
    free(m->name);
    free(m);
    
    return true;
}

bool edit_merch(db_t *db, char *name, char *new_name, char *new_desc, int new_price){
    
    elem_t merch_elem;
    
    bool found = ioopm_hash_table_lookup(db->name_to_merch, ptr_elem(name), &merch_elem);
    if (found == false){
        return false;
    }

    merch_t *m = merch_elem.p;

    bool name_changes = strcmp(name, new_name) != 0;  // stöd av gpt här, om namnet ändras behöver vi även ändra location to name hashtable.
    if (name_changes) {
        if (ioopm_hash_table_lookup(db->name_to_merch, ptr_elem(new_name), NULL)){
            return false;
        }
    
        ioopm_hash_table_remove(db->name_to_merch, ptr_elem(name), NULL);

        free(m->name);
        m->name = strdup(new_name);

        ioopm_hash_table_insert(db->name_to_merch, ptr_elem(m->name), ptr_elem(m));
        
        ioopm_list_t *locations = m->locations;
        size_t size = ioopm_linked_list_size(locations);
        for(size_t i = 0; i < size; i++){
            stock_entry_t *entry = ioopm_linked_list_get(locations, i).p;

            ioopm_hash_table_remove(db->loc_to_name, ptr_elem(entry->location), NULL);

            ioopm_hash_table_insert(db->loc_to_name, ptr_elem(entry->location), ptr_elem(m->name));
        }
    }

    free(m->desc);
    m->desc = strdup(new_desc);

    m->price = new_price;

    return true;
}

merch_t *merch_lookup(db_t *db, char *merch){  
    
    elem_t result;
    bool finding_merch = ioopm_hash_table_lookup(db->name_to_merch, ptr_elem(merch), &result);
            
    if(!finding_merch)
    {
        return NULL;
    }
    return result.p; //vi tar ut merchen
}

bool replenish(db_t *db, char *loc, char *name, int quantity) {
    merch_t *m = merch_lookup(db, name);
    if (!m) return false;

    // Skapa stock_entry
    stock_entry_t *entry = calloc(1, sizeof(stock_entry_t));
    entry->location = strdup(loc);
    entry->quantity = quantity;

    // Lägg till i merch->locations
    ioopm_linked_list_append(m->locations, ptr_elem(entry));

    // Uppdatera loc_to_name hash
    ioopm_hash_table_insert(db->loc_to_name, ptr_elem(entry->location), ptr_elem(m->name));

    return true;
}


cart_t *create_cart(db_t *db){
    cart_t *new_cart = calloc(1, sizeof(cart_t));
    new_cart->index = db->next_cart_id;
    db->next_cart_id++;
    new_cart->items = ioopm_hash_table_create(string_knr_hash, string_key_eq, NULL);

    ioopm_linked_list_append(db->carts, ptr_elem(new_cart));

    return new_cart;
}

bool remove_cart(db_t *db, int cart_index){
  size_t size = ioopm_linked_list_size(db->carts);

    for (size_t i = 0; i < size; i++)
    {
        cart_t *cart = ioopm_linked_list_get(db->carts, i).p;
        if (cart->index == cart_index)
        {
            ioopm_linked_list_remove(db->carts, i);
            ioopm_hash_table_destroy(cart->items);
            free(cart);
            return true;
        }
    } return false;
}



cart_t *cart_lookup(db_t *db, int cart_id){
    size_t size = ioopm_linked_list_size(db->carts);
    for(size_t i = 0; i < size; i++){   
        elem_t elem = ioopm_linked_list_get(db->carts, i);
        cart_t *cart = elem.p; 
        if (cart->index == cart_id)
        {
            return cart;
        }
    }
    return NULL;
}


bool add_to_cart(db_t *db, int cart_id, char *merch_name, int quantity)
{
    if (quantity <= 0)
    {
        printf("Error: Quantity must be positive.\n");
        return false;
    }

    cart_t *cart = cart_lookup(db, cart_id);
    if (cart == NULL)
    {
        printf("Error: No cart with id %d was found.\n", cart_id);
        return false;
    }

    merch_t *merch = merch_lookup(db, merch_name);
    if (merch == NULL)
    {
        printf("Error: The given merch: %s was not found\n", merch_name);
        return false;
    }


    int total_in_carts = 0;
    elem_t key = ptr_elem(merch->name);

    size_t cart_count = ioopm_linked_list_size(db->carts);
    for (size_t i = 0; i < cart_count; i++)
    {
        elem_t elem = ioopm_linked_list_get(db->carts, i);
        cart_t *c = elem.p;

        elem_t val;
        if (ioopm_hash_table_lookup(c->items, key, &val))
        {
            total_in_carts += val.i;
        }
    }


    int total_stock = 0;
    size_t loc_count = ioopm_linked_list_size(merch->locations);
    for (size_t i = 0; i < loc_count; i++)
    {
        stock_entry_t *entry = ioopm_linked_list_get(merch->locations, i).p;
        total_stock += entry->quantity;
    }

    int available = total_stock - total_in_carts;
    if (available < quantity)
    {
        printf("We don't have as many as you wish, you wanted %d of our %s, but we only have %d\n",
               quantity, merch_name, available);
        return false;
    }


    elem_t old_val;
    if (ioopm_hash_table_lookup(cart->items, key, &old_val))
    {
        ioopm_hash_table_insert(cart->items, key, int_elem(old_val.i + quantity));
    }
    else
    {
        ioopm_hash_table_insert(cart->items, key, int_elem(quantity));
    }

    return true;
}




bool remove_from_cart(db_t *db, int cart_id, char *merch, int quantity){
    cart_t *existing_cart = cart_lookup(db, cart_id);
    if(existing_cart == NULL)
    {
        printf("Error: No cart with id %d was found.\n", cart_id);
        return false;
    } 
   
    elem_t quantity_merch;
    if(!ioopm_hash_table_lookup(existing_cart->items, ptr_elem(merch), &quantity_merch)){ 
        printf("Error: The given merch: %s was not found in your cart\n", merch);
        return false;
    } 

    if(quantity > quantity_merch.i){
        printf("Error: You have less of %s in your cart than you want to remove\n", merch);
        return false;
    }
    int new_total = quantity_merch.i - quantity;
    if(new_total > 0){
        ioopm_hash_table_insert(existing_cart->items, ptr_elem(merch), int_elem(new_total));
    } else {
        ioopm_hash_table_remove(existing_cart->items, ptr_elem(merch), NULL); 
    }

    return true;
}


int calc_cost(db_t *db, int cart_id){
    int total_cost = 0;
    cart_t *existing_cart = cart_lookup(db, cart_id);
    if(existing_cart == NULL){
        printf("Cart ID does not exist\n");
        return 0;
    }

    ioopm_list_t *keys_list = ioopm_hash_table_keys(existing_cart->items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(keys_list);

    while(ioopm_iterator_has_next(iter)){
        elem_t key = ioopm_iterator_next(iter);               // key är elem_t för nyckeln som användes i ht
        elem_t value;
        if(ioopm_hash_table_lookup(existing_cart->items, key, &value)){ 
            int quantity = value.i;                            // antal i vagnen
            merch_t *m = merch_lookup(db, key.p);              
            if (m != NULL) {
                total_cost += quantity * m->price;            
            } else {
                // Om varan plötsligt inte finns i DB -> betrakta pris som 0 och skriv varning
                printf("Warning: merch '%s' missing from DB when calculating cost\n", (char*)key.p);
            }
        }
    }

    ioopm_iterator_destroy(iter);             
    ioopm_linked_list_destroy(keys_list);       
    return total_cost;
}




bool checkout(db_t *db, int cart_id){
    cart_t *checkout_cart = cart_lookup(db, cart_id);
    if(checkout_cart == NULL){
        printf("Cart ID does not exist\n");
        return false;
    }

    ioopm_list_t *keys_list = ioopm_hash_table_keys(checkout_cart->items);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(keys_list);

    while(ioopm_iterator_has_next(iter)){
        elem_t key = ioopm_iterator_next(iter);
        elem_t value;
        int want = 0;
        if(ioopm_hash_table_lookup(checkout_cart->items, key, &value)){
            want = value.i;
        }

        // Hitta merch i DB
        merch_t *current_merch = merch_lookup(db, key.p);
        if (current_merch == NULL){
            printf("Error: merchandise '%s' not found in DB during checkout\n", (char*)key.p);
            continue; // fortsätt med nästa artikel (eller hantera som fel beroende på krav)
        }

        
        size_t size = ioopm_linked_list_size(current_merch->locations);
        int remaining = want;
        for(size_t i = 0; i < size && remaining > 0; i++){
            elem_t e = ioopm_linked_list_get(current_merch->locations, i);
            stock_entry_t *entry = e.p;

            if (entry->quantity >= remaining){
                entry->quantity -= remaining;
                if (entry->quantity == 0) {// om location nu är tom, uppdatera loc_to_name (ta bort) — valfritt men bra, detta hjälpte chat med men tänker att det kan vara bra att ha kvar så man inte får något fel senare
                    ioopm_hash_table_remove(db->loc_to_name, ptr_elem(entry->location), NULL);
                }
                remaining = 0;
            } else {
                // ta allt från denna location och fortsätt
                remaining -= entry->quantity;
                entry->quantity = 0;
                ioopm_hash_table_remove(db->loc_to_name, ptr_elem(entry->location), NULL);//samma som ovan
            }
        }

        if (remaining > 0){
            // Detta borde inte hända eftersom add_to_cart garanterar tillräckligt lager,
            // men hantera fallet ändå:
            printf("Warning: during checkout, could not fulfil %d of '%s' (short by %d)\n",
                   want, (char*)key.p, remaining);
            // valfritt: här kan du försöka backa tidigare ändringar eller abortera checkout
        }
    }

    // Ta bort kundvagnen helt från systemet (efter att lagret minskats)
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(keys_list);

    // Sök cart och ta bort den (remove_cart tar hand om att free:a cart->items och cart)
    if (!remove_cart(db, cart_id)){
        // Om removal misslyckas: skriv debug-varning
        printf("Warning: failed to remove cart %d after checkout\n", cart_id);
        return false;
    }

    return true;
}

#include "backend.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"        
#include "hash_table.h"
#include "linked_list.h"






void ui_edit_merch(db_t *db){
    char *name = ask_question_string("Please insert name of merchandise you would like to edit: ");
    char *new_name = ask_question_string("Insert new name of merchandise: ");
    char *new_desc = ask_question_string("Insert new description of merchandise: ");
    int new_price = ask_question_int("Insert new price of merchandise: ");
    char *confirm = ask_question_string("Confirm edit? [Y]es or [N]o: ");

    bool edit_confirmed = (confirm[0] == 'Y' || confirm[0] == 'y');

    if (edit_confirmed) {
        if (edit_merch(db, name, new_name, new_desc, new_price)) {
            printf("Merchandise updated.\n");
        } else {
            printf("Failed to update merchandise (not found or new name already exists).\n");
        }
    } else {
        printf("Edit cancelled.\n");
    }
    
    free(name);
    free(new_name);
    free(new_desc);
    free(confirm);
}


void ui_show_stock(db_t *db){

    char *name = ask_question_string("Please input name of merchandise to show stock: \n");

    elem_t merchandise;
    bool result = ioopm_hash_table_lookup(db->name_to_merch, ptr_elem(name), &merchandise);

    if(!result){
        printf("Merchandise with name '%s' does not exist\n", name);
        free(name);
        return;
    }


    merch_t *merch = merchandise.p;   // pekarfältet i vår elem innehåller ju pekaren till merch
    ioopm_list_t *locations = merch->locations;
    size_t size = ioopm_linked_list_size(locations);
    
    printf("For item %s there is:\n", name);

    for(size_t i = 0; i < size; i++){
        stock_entry_t *entry = ioopm_linked_list_get(locations, i).p;  // vi tar ut en elem_t och tar sedan pointer fältet av den, för där är en pekare till rätt stock_entry struct
        printf("In location %s: %d items\n", entry->location, entry->quantity);
    }

    free(name);
}

void ui_replenish(db_t *db){
    char *loc = ask_question_string("Enter storage location to replenish: \n");
    bool valid_shelf = is_valid_shelf(loc);
    if(!valid_shelf){
        printf("Shelf is not valid, it must be written like A12\n");
        return;
    }
    char *merch = ask_question_string("Enter what merch to replenish: \n");
    merch_t *valid_merch = merch_lookup(db, merch);
    if(valid_merch == NULL){
        printf("This merch is not in the store!\n");
        return;
    }
    if(!strcmp(merch, valid_merch->name) != 0){
        printf("This merch is not in the store!\n");
        return;
    }
    int quantity = ask_question_int("Enter how many to add to stock: \n");
    if(!replenish(db, loc, merch, quantity)){
        printf("ERROR: could not add merch to stock\n");
    }
    printf("%d of %s was successfully added to stock: %s\n", quantity, merch, loc);

    free(loc);
    free(merch);
}


void ui_list_merch(db_t *db){
    
    ioopm_list_t *keys = ioopm_hash_table_keys(db->name_to_merch);
    size_t total = ioopm_linked_list_size(keys);

    if(total == 0){
        printf("Currently no merchandise in store\n");
        ioopm_linked_list_destroy(keys);
        return;
    }

    for(size_t i = 0; i < total; i++){
        char *name = ioopm_linked_list_get(keys, i).p;
        printf("%s\n", name);

        if ((i + 1) % 20 == 0 && (i + 1) < total){
            char *result = ask_question_string("Continue the listing? Press Enter or [Y] to continue, N to stop: ");
            bool stop = false;
            if(result[0] == 'N' || result[0] == 'n'){
                stop = true;
            }
            free(result);
            if (stop) break;
        }
    }

    ioopm_linked_list_destroy(keys);
}

void ui_remove_merch(db_t *db){
    char *name = ask_question_string("Please input name of the merchandise to be removed: ");
    bool result = remove_merch(db, name);
    
    if (!result) {
        printf("Merchandise does not exist!\n");
    }

    if(result) {
        printf("Merchandise removed succesfully\n");
    }

    free(name);
}


void ui_add_merch(db_t *db){
    char *name = ask_question_string("Please input name of the merchandise to be added: ");
    char *desc = ask_question_string("Please input description of the merchandise: ");
    int price = ask_question_int("Please input price of the merchandise: ");

    bool result = add_merch(db, name, desc, price);
    if (!result) {
        printf("Failed to add merchandise, merch with same name already exists\n");
    }

    if(result) {
        printf("Merchandise added successfully!\n");
    }

    free(name);
    free(desc);
}

void ui_create_cart(db_t *db){
    cart_t *new_cart = create_cart(db);
    printf("your cart-number is %d\n", new_cart->index);
}

void ui_remove_cart(db_t *db){
    int cart_id = ask_question_int("Enter the ID of the cart you want to remove: \n");
    bool success = remove_cart(db, cart_id);
    if(!success){
        printf("ERROR: Could not remove cart with ID %d\n", cart_id);
        return;
    }
    printf("Cart with ID %d was successfully removed!\n", cart_id);
}


void ui_add_to_cart(db_t *db)
{
    int cart_id = ask_question_int("Enter cart ID: \n");
    char *merch_name = ask_question_string("Enter merchandise name:\n ");
    int quantity = ask_question_int("Enter quantity: \n");

    bool success = add_to_cart(db, cart_id, merch_name, quantity);

    if (!success)
    {
        printf("Failed to add item to cart.\n");
    }

    if(success){
        printf("Successfully added %d of '%s' to cart %d!\n", quantity, merch_name, cart_id);
    }

    free(merch_name);
}

void ui_remove_from_cart(db_t *db){
   int cart_id = ask_question_int("Enter the cart ID of the cart you want to remove an item from:\n ");
   char *merch_name = ask_question_string("Enter the merchandise you want to remove: \n");
   int quantity = ask_question_int("Enter how many you want to remove: \n");

   bool success = remove_from_cart(db, cart_id, merch_name, quantity);
   if(success){
    printf("%d of %s was sucessfully removed from cart %d\n", quantity, merch_name, cart_id);
   }
   else{
    printf("ERROR: could not remove %d of %s successfully from cart %d\n+", quantity, merch_name, cart_id);
   }

   free(merch_name);
}

void ui_calc_cost(db_t *db){
    int cart_id = ask_question_int("Enter the cart ID of the cart you want to do a checkout on:\n");
    int price = calc_cost(db, cart_id);
    if(price == -1){// Skulle kunna visa på redovisning!!
        printf("ERROR: Could not calculate cost of cart with ID %d\n", cart_id);
    }
    printf("Total cost of cart with ID %d is; %d\n", cart_id, price);
}

void ui_checkout(db_t *db){
    int cart_id = ask_question_int("Enter the cart ID of the cart you want to do a checkout on:\n");
    int price = calc_cost(db, cart_id);
    if(!checkout(db, cart_id)){
        printf("ERROR: Could not checkout cart with ID %d", cart_id);
    }
    printf("Checkout was successfull and the total price of cart with ID %d was: %d\n", cart_id, price);
}

bool ui_quit(){
    char *answer = ask_question_string("[Y] if you want to quit, [N] if not\n");
    bool confirm = false;
   
    if (answer[0] == 'Y' || answer[0] == 'y'){
        confirm = true;
    }
   
    free(answer);
    return confirm;
}

void print_menu(void)
{
    printf(
        "[A] To add new merchandise\n"
        "[D] To remove merchandise\n"
        "[L] To list all merchandise\n"
        "[E] To edit merchandise\n"
        "[S] To show stock\n"
        "[P] To replenish\n"
        "[C] To create a shopping cart\n"
        "[R] To remove a shopping cart\n"
        "[+] To add merch to shopping cart\n"
        "[-] To remove merch from shopping cart\n"
        "[=] To calculate costs of a shopping cart\n"
        "[O] To checkout\n"
        "[Q] To quit\n"
    );
}

char ask_question_menu(void)
{
    print_menu();
    char input;

    while (1)
    {
        printf("> ");  // optional prompt
        if (scanf(" %c", &input) != 1) {
            // input error – flush and retry
        }

        input = toupper((unsigned char) input);

        // flush rest of line
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }

        if (input == 'A' || input == 'D' || input == 'L' ||
            input == 'E' || input == 'S' || input == 'P' ||
            input == 'C' || input == 'R' || input == '+' ||
            input == '-' || input == '=' || input == 'O' ||
            input == 'Q')
        {
            return input;
        }
        else
        {
            printf("Wrong input, please choose one of the options.\n");
        }
    }
}

void run_ui(db_t *db){
    while(1){
        char action = ask_question_menu();
        switch(action){
            case 'A': ui_add_merch(db); break;    // klar
            case 'D': ui_remove_merch(db); break; // klar
            case 'L': ui_list_merch(db); break;   // klar
            case 'E': ui_edit_merch(db); break;   // klar
            case 'S': ui_show_stock(db); break;   // klar
            case 'P': ui_replenish(db); break;
            case 'C': ui_create_cart(db); break;
            case 'R': ui_remove_cart(db); break;
            case '+': ui_add_to_cart(db); break;
            case '-': ui_remove_from_cart(db); break;
            case '=': ui_calc_cost(db); break;
            case 'O': ui_checkout(db); break; 
            case 'Q': 
                if (ui_quit()) return;            // klar  // om användaren ej bekräftar quit, fortsätter loopen.
                break;                        
        }
    }
}

void main(){
    db_t *db = db_create();
    run_ui(db);
}
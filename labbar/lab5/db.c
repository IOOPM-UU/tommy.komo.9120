#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"


struct item{
    char *name;
    char *desc;
    int price;
    char *shelf;
};

typedef struct item item_t;

void print_item(item_t *item){
    int kronor = item->price / 100;
    int oren = item->price % 100;
    printf("Name: %s\n",item->name);
    printf("Desc: %s\n",item->desc);
    printf("Price: %d.%02d SEK\n",kronor,oren);
    printf("Shelf: %s\n",item->shelf);
}

item_t make_item(char *name, char*desc, int price, char *shelf){
    if(strlen(shelf)==3 && isalpha(shelf[0]) && isdigit(shelf[1]) && isdigit(shelf[2])){
        item_t item = {.name = name, .desc = desc, .price = price, .shelf = shelf};
        return item;
    }
    printf("Incorrect shelf, please input letter followed by two digits.\n");
    item_t empty_item = {0}; //need to return something
    return empty_item;
}

bool is_shelf(char *str){
    if(strlen(str)==3 && isalpha(str[0]) && isdigit(str[1]) && isdigit(str[2])){
        return true;
    }
    return false;
}

char *ask_question_shelf(char *question){
    do{
        char *shelf = ask_question_string(question);
        if(is_shelf(shelf)){
            return shelf;
        }
        printf("Wrong format, shelfs are a letter followed by two integers(A25)\n");
        free(shelf);
        } while(1);
}

item_t input_item(){
    char *name = ask_question_string("Please input name of item: ");
    char *desc = ask_question_string("Please input desc of item: ");
    int price = ask_question_int("Please input price of item: ");
    char *shelf = ask_question_shelf("Please input shelf of item: ");
    item_t item = {.name = name, .desc = desc, .price = price, .shelf = shelf};
    return item;
}

char *magick(char *array1[],char *array2[], char *array3[], int length){
    char buf[255];
    char *word1 = array1[rand() % length];
    char *word2 = array2[rand() % length];
    char *word3 = array3[rand() % length];
    sprintf(buf, "%s-%s %s",word1,word2,word3);
    return strdup(buf);
}

void list_db(item_t *items, int no_items){
    for(int i=0;i < no_items;i++){
        printf("%d. %s\n",i+1,items[i].name);
    }
}

void edit_db(item_t *items, int db_siz){
    int item_number = 0;
    
    do {
        list_db(items,db_siz);
        item_number = ask_question_int("Which item would you like to edit? ");
        if (item_number < 1 || item_number > db_siz) {
            printf("Please choose an item number between 1 and %d.\n", db_siz);
        }
    } while (item_number < 1 || item_number > db_siz);
    
    print_item(&items[item_number-1]);
    item_t new_item = input_item();
    items[item_number-1] = new_item;
}

void print_menu(){
    printf("[L]ägga till en vara\n"
           "[T]a bort en vara\n"
           "[R]edigera en vara\n"
           "Ån[g]ra senaste ändringen\n"
           "Lista [h]ela varukatalogen\n"
           "[A]vsluta\n");
}

char ask_question_menu(){
    print_menu();
    char input;
    while(1){
        scanf(" %c", &input);
        input = toupper(input);

        int c;
        while((c = getchar()) != '\n' && c != EOF);

        if (input == 'L' || input == 'T' || input == 'R' || 
            input == 'G' || input == 'H' || input == 'A'){
                return input;
            }
        else{
            printf("Wrong input, please choose L,T,R,G,H or A\n");
        }
    }
}

void add_item_to_db(item_t *items, int *db_siz){
    item_t new_item = input_item();
    items[*db_siz] = new_item;
    (*db_siz)++;
}

void remove_item_from_db(item_t *items, int *db_siz){
    list_db(items, *db_siz);
    int item_number;
    do {
        item_number = ask_question_int("Which item would you like to delete? ");
        if (item_number < 1 || item_number > *db_siz) {
            printf("Please choose an item number between 1 and %d.\n", *db_siz);
        }
    } while (item_number < 1 || item_number > *db_siz);
    
    if(item_number != *db_siz){
        for (int i = item_number;i <= *db_siz;i++){
            items[i-1] = items[i];
        }
    }
    
    
    (*db_siz)--;
}

int event_loop(item_t *items, int *db_siz) {
    while (1) {
        char input = ask_question_menu();
        if (input == 'L') {
            add_item_to_db(items, db_siz);
        } else if (input == 'T') {
            remove_item_from_db(items, db_siz);
        } else if (input == 'R') {
            edit_db(items, *db_siz);
        } else if (input == 'G') {
            printf("Functionality not yet implemented\n");
        } else if (input == 'H') {
            list_db(items, *db_siz);
        } else if (input == 'A') {
            return 1; // Exit the event loop
        }
    }
}

int main() {
    item_t db[16];
    int db_siz = 0;

    while (1) {
        if (event_loop(db, &db_siz)) {
            printf("Exiting program...\n");
            break;
        }
    }

    return 0;
}
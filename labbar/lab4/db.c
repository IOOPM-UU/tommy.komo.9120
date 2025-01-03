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
        item_number = ask_question_int("Which item would you like to edit? ");
        if (item_number < 1 || item_number > db_siz) {
            printf("Please choose an item number between 1 and %d.\n", db_siz);
        }
    } while (item_number < 1 || item_number > db_siz);
    
    print_item(&items[item_number-1]);
    item_t new_item = input_item();
    items[item_number-1] = new_item;
}



int main(int argc, char *argv[])
{
  char *array1[] = {"Laser", "Polka", "Extra"}; 
  char *array2[] = {"snyggt", "gott", "mediokert"}; 
  char *array3[] = {"skruvmejsel", "fanta", "deadline"}; 

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3,3); 
        char *desc = magick(array1, array2, array3,3); 
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, shelf);

        db[db_siz] = item;
        ++db_siz;
      }

     list_db(db, db_siz);

  }
  return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"
#include "common.h"

extern char *strdup(const char *);
answer_t ask_question(char *question, check_func *check, convert_func *convert);


void print(char *string){
    int counter = 0;
    int length = strlen(string);

    while(counter < length){
        putchar(string[counter]);
        counter++;
    }
}

void println(char *string){
    print(string);
    putchar('\n');
}


bool is_number(char *str)
{
  int len = strlen(str);
  int start = 0;
  if (str[0] == '-'){
    if (len == 1) return false;
    start = 1;
  }


  for (int i = start; i < len; i++){
    if (isdigit(str[i]) == false){
        return false;
    }
  }
    return true;
}


bool not_empty(char *str)
{
  return strlen(str) > 0;
}



int read_string(char *buf, int buf_siz){
  int counter = 0;
  int c = getchar();

  while (c != '\n' && c != EOF){
    if(counter < buf_siz -1){
    buf[counter] = c;
    counter++;  
    }
    c = getchar();
  } 
  
  
  buf[counter] = '\0';
  return counter; 
}


answer_t ask_question(char *question, check_func *check, convert_func *convert){
    char buf[400];
    int len = 0;

    do {
        printf("%s",question);
        len = read_string(buf, sizeof buf);

    } while(len < 1 || check(buf) != true);

    return convert(buf);

}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value; 
}

unsigned long string_knr_hash(elem_t key)  // fått av GPT
{
    unsigned long hash = 0;
    char *str = key.p;   // elem_t stores the string pointer in .p
    int c;

    while ((c = *str++))
    {
        hash = c + 31 * hash;
    }

    return hash;
}

bool string_key_eq(elem_t key1, elem_t key2)
{
    return strcmp((char *) key1.p, (char *) key2.p) == 0;
}
#ifndef __UTILS_H__
#define __UTILS_H__


#include <stdbool.h>
#include "common.h"
extern char *strdup(const char *);

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef int int_fold_func(int, int);
typedef bool check_func(char *);
typedef answer_t convert_func(char *);

bool not_empty(char *str);
int read_string(char *buf, int buf_siz);
bool is_number(char *str);
answer_t ask_question(char *question, check_func *check, convert_func *convert);
char *ask_question_string(char *question);
int ask_question_int(char *question);
void print(char *string);
void println(char *string);

unsigned long string_knr_hash(elem_t key);
bool string_key_eq(elem_t key1, elem_t key2);
#endif 
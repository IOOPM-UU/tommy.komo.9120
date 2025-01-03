#include <stdio.h>
#include <string.h>
#include "utils.h"

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert){
  char buf[400];
  do{
    printf("%s",question);
    read_string(buf,400);
  }while (!check(buf));  
  
  return convert(buf);
}

int ask_question_int(char *question)
{

  int result = 0;
  int conversions = 0;
  do
    {
      printf("%s\n", question);
      conversions = scanf("%d", &result);
      int c;
      do
        {
          c = getchar();
        }
      while (c != '\n' && c != EOF);
      putchar('\n');
    }
  while (conversions < 1);
  return result;
}

int read_string(char *buf, int buf_siz){
  
  int counter = 0;
  char c;

  while(counter < (buf_siz-1)){
    c = getchar();
    
    if(c == '\n' || c == EOF ) {
      break;
    }
    buf[counter] = c;
    counter++;
  }
  buf[counter] = '\0';

  return counter; 
}


char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}

/*
char *ask_question_string(char *question, char *buf, int buf_siz){
  int counter = 0;
  
  while(counter == 0){
    printf("%s", question);
    counter = read_string(buf, buf_siz);
    if (counter == 0) {
      printf("Please write a question\n");
    }
  }

  return buf;
}
*/
void println(char *string){
  puts(string);
  printf("\n");
}

void print(char *string){
  int counter = 0;
  while(string[counter] != '\0'){
    putchar(string[counter]);
    counter++;
  }
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool is_number(char *str)
{
  if (str[0] == '-' && strlen(str) == 1){   // Om det bara är ett minustecken
    return false;  
  }
  int start = 0;
  if (str[0] == '-'){
    start = 1;
  }
  
  for (int i = start; i <strlen(str); i++){
    if (isdigit(str[i]) == false) {
      return false;
      }
  }
  
    return true;
}



int main(int argc, char *argv[])
{
if (argc > 1 && is_number(argv[1]))
{
  printf("%s is a number\n", argv[1]);
}
else
{
  if (argc > 1)
  {
    printf("%s is not a number\n", argv[1]);
  }
  else
  {
    printf("Please provide a command line argument!\n");
  }
}

}



#include <stdio.h>
#include <string.h>

int string_length(char *string){
  int counter = 0;
  while(string[counter] != '\0'){
    counter++;
  }
  return counter;
}

void print(char *string){
  int counter = 0;
  while(string[counter] != '\0'){
    putchar(string[counter]);
    counter++;
  }
}

void println(char *string){
  puts(string);
  printf("\n");
}



int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s words or string", argv[0]);
  }
  else
  {
    print("tjabbatjena");
    puts("tjabbatjena");
  }
  return 0;
}

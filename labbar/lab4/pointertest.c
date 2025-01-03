#include <stdio.h>


void swap(int *a, int *b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void print(char *str)
{
  while(*str != '\0'){
    putchar(*str);
    ++str;
  }
}




int main(void)
{

  int a = 3;
  int b = 6;
  printf("a = %d, b = %d\n",a,b);
  swap(&a, &b);
  printf("a = %d, b = %d\n",a,b);
  
  
  
  return 0;
}
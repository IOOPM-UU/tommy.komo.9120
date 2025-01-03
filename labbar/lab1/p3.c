#include <stdio.h>

int main(void)
{
  int totalAsterix; 
  for(int i=1;i<11;i++)
  {
    int x = 0;
    while(x < i)
    {
      printf("*");
      x++;
      totalAsterix++;
    }
    printf("\n");
  }
  printf("Totalt: %d\n", totalAsterix);
  return 0;
}


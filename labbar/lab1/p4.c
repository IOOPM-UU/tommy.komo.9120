#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int arguments = argc;
  if (arguments < 2 || arguments > 3)
  {
    printf("Incorrect input\n");
  }
  else{

  int totalAsterix; 
  int rows = atoi(argv[1]);
  int mult = atoi(argv[2]);
  
  for(int i=1;i<(rows+1);i++)
  {
    int x = 0;
    int y = mult * i ;
    while(x < y)
    {
      printf("*");
      x++;
      totalAsterix++;
    }
    printf("\n");
  }
  printf("Totalt: %d\n", totalAsterix);
  }
  return 0;
}



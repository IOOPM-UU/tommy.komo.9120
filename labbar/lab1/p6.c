#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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


int gcd(int a, int b) {
    if (a % b == 0){
        printf("gcd(%d, %d) = %d\n", a,b,b); 
        return 0;
    }
    
    int aTemp = a;
    int bTemp = b;
    
    while (aTemp % bTemp != 0){
        int bNew = aTemp % bTemp;
        aTemp = bTemp;
        
        if (aTemp % bNew == 0){
            printf("gcd(%d, %d) = %d\n", a, b, bNew);
            return  0;
        } else {
            bTemp = aTemp % bNew;
        }
    
    }

    printf("gcd(%d, %d) = %d\n", a, b, bTemp);
}

int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        printf("Usage: %s int1 int2\n", argv[0]);
        return 1;           // Returnera felkod 1 om antalet argument är felaktigt
    }

    if(!is_number(argv[1]) || !is_number(argv[2])){
        printf("Use numbers only\n");
        return 1;
    }
    
    if(atoi(argv[1]) < 0 || atoi(argv[2]) < 0){
        printf("Inputs must be positive integers\n");
        return 1;
    }

    gcd(atoi(argv[1]), atoi(argv[2]));
    return 0;
}


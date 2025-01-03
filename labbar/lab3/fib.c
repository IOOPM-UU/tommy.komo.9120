#include <stdio.h>
#include <stdlib.h>

int rec_fib(int n){
  if(n == 0) return 0;
  if(n == 1) return 1;

  return rec_fib(n-1) + rec_fib(n-2);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: %s number\n",argv[0]);
    }
    else{
        int n = atoi(argv[1]);
        if (n < 2){
            printf("fib(%d) = %d\n",n,n);
        }
        else{
            printf("fib(%s) = %d\n",argv[1],rec_fib(n));
        }
    }
    return 0;
}
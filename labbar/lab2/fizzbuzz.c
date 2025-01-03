#include <stdio.h>
#include <stdlib.h>

void print_number(int num){
    
    if(num % 3 == 0 && num % 5 == 0){
        printf("Fizz Buzz");
    }
    else if(num % 3 == 0){
        printf("Fizz");
    }
    else if(num % 5 == 0){
        printf("Buzz");
    }
    
    else{
        printf("%d",num);
    }

}


int main(int argc, char *argv[]){
    
    if(argc =! 2){
        printf("Use: \aout integer");
        return 1;
    }

    for(int i = 1; i <= atoi(argv[1]);i++){
        print_number(i);
        if(i != atoi(argv[1])){
            printf(", ");
        }
    }
    
    printf("\n");
    
    return 0;
    
    } 







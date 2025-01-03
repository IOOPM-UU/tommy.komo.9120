#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int is_prime(int n) {
    if (n <= 1) {                   // 0 och 1 är inte primtal
        return 0;  
    }

    int limit = floor(sqrt(n)) + 1;

    for (int i = 2; i < limit; ++i) {
        if (n % i == 0) {
            return 0;               // n är delbart med i, så det är inte ett primtal
        }
    }

    return 1;                       // n är ett primtal
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Usage: %s number\n", argv[0]);
        return 1;           // Returnera felkod 1 om antalet argument är felaktigt
    }

    
    // Konvertera kommandoradsargumentet till ett heltal
    int number = atoi(argv[1]);          
    // Avgör om det angivna talet är ett primtal och skriver ut resultatet
    if (is_prime(number)) {
        printf("%d is a prime number\n", number);
    } else {
        printf("%d is not a prime number\n", number);
    }

    return 0;
}






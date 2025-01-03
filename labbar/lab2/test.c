#include <stdio.h>

int main()
{
    int age;
    char name[1024];

    printf("Enter Age: ");
    scanf("%d", &age);

    

    printf("Enter name: ");
    fgets(name, 1024, stdin);

    printf("Age: %d\n", age);
    printf("Name: %s", name);


    return 0;
}
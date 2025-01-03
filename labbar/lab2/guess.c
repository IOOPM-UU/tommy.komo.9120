#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <time.h>

int main() {
    srandom(time(NULL));
    int r = random() % 1024;
    char question[] = "Skriv in ditt namn:";
    char *name = ask_question_string(question);


    char question2[200];
    sprintf(question2, "Du %s jag tänker på ett tal ... kan du gissa vilket?", name);

    int gissning;
    gissning = ask_question_int(question2);
    
    int counter = 0;
    
    for(int i=0;i<=15;i++){
        counter++;
        if(counter == 15){
            printf("Nu har du slut på gissningar! Jag tänkte på %d\n",r);
            break;
        }
        if(gissning == r){
            printf("Bingo!\n");
            printf("Det tog %s %d gissningar att komma fram till %d\n",name,counter,r);
            break;
        }
        if(gissning < r){
            gissning = ask_question_int("För litet!");
        }
        if(gissning > r){
            gissning = ask_question_int("För stort!");
        }
    }
    return 0;
}





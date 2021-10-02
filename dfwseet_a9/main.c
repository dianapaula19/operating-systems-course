#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
/*
A9. (2 puncte) Scrieti un program cara arata ce se intampla daca mai multe 
procese care se ruleaza in paralel in foreground incearca in acelasi timp
sa citeasca de la tastatura un caracter (fiecare il va citi, sau doar unul
aleator il va citi, etc.); procesele respective vor fi fii cu fork ai 
programului initial. Nu se vor folosi decat functii de la nivelui inferior
de prelucrare a fisierelor.
*/

int main() {
    char c;
    pid_t pid1;
    pid_t pid2;
    pid1 = fork();
    pid2 = fork();
    read(0, &c, 1);
    write(1, &c, 1);

}

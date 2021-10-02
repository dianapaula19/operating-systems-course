/*
4. (2 puncte) Program care armeaza ceasul ("alarm") pentru 2 secunde, apoi 
intra intr-un ciclu infinit. La primirea lui SIGALRM nu se va termina 
procesul ci se va iesi din ciclu (se va continua programul dupa ciclul 
respectiv).
*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

volatile int end = 0;

void h_alarm(int signal) {
    end = 1;
}

int main() {
    signal(SIGALRM, h_alarm);
    alarm(2);
    if (errno != 0) {
        perror("Error");
        return EXIT_FAILURE;
    }
    while(1) {
        if (end == 1) break;
    } 
    
    fprintf(stdout, "Got out of the loop!\n");
    return EXIT_SUCCESS;
}
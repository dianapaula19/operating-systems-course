#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#define MAX_SIG 100
pid_t op;
int no_sig_sent = 0;

void h(int sig) {
    signal(sig, h);
}

int main() {
    signal(SIGUSR1, h);
    fprintf(stdout, "My PID: %d\n", getpid());
    fprintf(stdout, "Other PID: ");
    fscanf(stdin, "%d", &op);
    srand(time(NULL));
    while(1) {
        int sig = rand() % 64;
        printf("The signal sent is: %d\n", sig);
        if (sig == SIGKILL || sig == SIGSTOP || sig == SIGCONT) continue;
        kill(op, SIGINT);
        pause();
        ++no_sig_sent;
        if (no_sig_sent == MAX_SIG) break;
        
    }
    kill(op, SIGKILL);
    return 0;
}
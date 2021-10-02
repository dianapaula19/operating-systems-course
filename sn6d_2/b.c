#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
pid_t op;
int max_signal = 0;

void h(int sig) {
    if (max_signal < sig) max_signal = sig;
    signal(sig, h);
    kill(op, SIGUSR1);
}
void h_kill(int sig) {
    fprintf(stdout, "The maximum value of the signal sent is: %d \n", max_signal);
}

int main() {
    for (int i = 1; i <= 64; ++i) {
        if (i == SIGKILL || i == SIGSTOP || i == SIGCONT || i == SIGUSR1) continue;
        signal(i, h);

    }
    signal(SIGINT, h);
    signal(SIGKILL, h_kill);
    fprintf(stdout, "My PID: %d\n", getpid());
    fprintf(stdout, "Other PID: ");
    fscanf(stdin, "%d", &op);
    while(1) pause();

    return 0;
}
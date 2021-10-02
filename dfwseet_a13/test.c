#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    int val = fork();
    if (val == 0) {
        printf("Child Process\n");
    } else {
        printf("Parent Process\n");
    }
    printf("Will i be in the clone process?\n");
   return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#define SIZE 7

int prim(int n) {
    if (n == 1) return 0;
    for (int d = 2; d < n; ++d) {
        if (n % d == 0) return 0;
    }
    return 1;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Utilizare: %s NUMAR\n", argv[0]);
        return EXIT_FAILURE;
    }
    char* endptr;
    int n = strtol(argv[1], &endptr, 10);
    if (errno != 0) {
        perror(argv[1]);
        return EXIT_FAILURE;
    } else if (*endptr != '\0' || n < 1) {
        fprintf(stderr, "%s: not a valid number\n", argv[1]);
        return EXIT_FAILURE;
    }

    char* p;
    if ((p = malloc(sizeof(char) * SIZE)) == NULL) {
        perror("Error");
        return EXIT_FAILURE;
    }
    if (prim(n) == 0) {
        strcpy(p, "neprim");
    } else {
        strcpy(p, "prim");
    }

    fprintf(stdout, "Numarul %s este %s\n", argv[1], p);
    return EXIT_SUCCESS;

}
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
/*
A13. (4 puncte) Scrieti un program care verifica daca un numar apare sau nu
intr-un vector (numarul se vectorul sunt dati ca argumente in linia de 
comanda) printr-o strategie de tip divide et impera: se imparte vectorul in
doua, apoi se initiaza cate un proces fiu care cauta numarul in fiecare
jumatate, in aceeasi maniera; cele doua procese se desfasoara in paralel. 
Fiecare subproces returneaza 0 = negasit, 1 = gasit. Fiecare proces nu se 
termina pana nu i se termina toti fii si in final returneaza suma valorilor
returnate de ei. Procesul initial trebuie in plus sa afiseze un mesaj de tip 
"gasit"/"negasit".
*/

void print_arr(long* arr, long size) {
    for (long i = 0; i < size; ++i) 
        printf("%ld ", arr[i]);
    printf("\n");

}


int find(long* arr, long l, long r, long num) {
    if (l == r) {
        if (arr[l] == num) return 1;
        else return 0;
    } else if (l < r) {
        long middle = (l + r) / 2;
        return find(arr, l, middle, num) + find(arr, middle + 1, r, num);
    } else {
        return 0;
    }
}

int find_fork(long *arr, long l, long r, long num) {
  long middle = (l + r) / 2;
  pid_t first_process;
  pid_t second_process;
  int status_one;
  int status_two;
  if (l < r) {
    first_process = fork();
    if (first_process < 0) {
        perror("Error");
        return EXIT_FAILURE;
    } else if (first_process == 0) {
      exit(find_fork(arr, l, middle, num));
    } else  {
        second_process = fork();
        if (second_process < 0) {
            perror("Error");
            return EXIT_FAILURE;
        } else if (second_process == 0) {
            exit(find_fork(arr, middle + 1, r, num));
        } else {
            printf("Parent Process: ");
            first_process = waitpid(first_process, &status_one, 0);
            second_process = waitpid(second_process, &status_two, 0);
            if (first_process > 0 && second_process > 0) {
                if (WIFEXITED(status_one) && WIFEXITED(status_two)) {
                    int found = WEXITSTATUS(status_one) + WEXITSTATUS(status_two);
                    if (found) printf("gasit\n");
                    else printf("negasit\n");
                    return found;
                }
            }
        }
    }
  } else {
    if (l == r) {
      if (arr[l] == num) return 1;
    }
  }

  return 0;
}


int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s NUMBER <ARRAY>", argv[0]);
        return EXIT_FAILURE;
    }
    
    char *endptr;
    long num = strtol(argv[1], &endptr, 10);
    if (errno != 0) {
        perror(argv[1]);
        return EXIT_FAILURE;
    } else if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "%s: Not a valid number\n", argv[1]);
        return EXIT_FAILURE;
    }

    long size = argc - 2;
    long *arr;
    
    if ((arr = malloc(sizeof(long) * size)) == NULL) {
        fprintf(stderr, "An error occurred while allocating the memory for the array\n");
        return EXIT_FAILURE;
    }
    
    long i = 2;
    
    while (argv[i]) {
        long num = strtol(argv[i], &endptr, 10);
        if (errno != 0) {
            perror(argv[i]);
            free(arr);
            return EXIT_FAILURE;
        } else if (endptr == argv[i] || *endptr != '\0') {
            fprintf(stderr, "%s: Not a valid number\n", argv[i]);
            free(arr);
            return EXIT_FAILURE;
        } else {
            arr[i - 2] = num;
            ++i;
        }
    }

    print_arr(arr, size);
    int sum = find(arr, 0, size - 1, num);

    printf("Solutie fara fork: \n");

    if (sum == 0) {
        printf("negasit\n");
    } else {
        if (sum == 1) {
            printf("gasit o data\n");    
        } else {
            printf("gasit de %d ori\n", sum);
        }
    }

    printf("Solutie cu fork: \n");

    sum = find_fork(arr, 0, size - 1, num);
    if (sum == 0) {
        printf("negasit\n");
    } else {
        if (sum == 1) {
            printf("gasit o data\n");    
        } else {
            printf("gasit de %d ori\n", sum);
        }
    }

    free(arr);
    return EXIT_SUCCESS;


}
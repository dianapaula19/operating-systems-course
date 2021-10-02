#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
/*
A5. (3.5 puncte) Scrieti o functie "getint" care primeste ca argument un 
descriptor de fisier (presupus a fi deschis in prealabil pentru citire) si 
citeste cu format zecimal un intreg din fisierul respectiv, returnand acel 
intreg. Functia nu va folosi decat functii de la nivelul inferior de 
prelucrare a fisierelor. Deci, daca se poate citi intregul, apelul:
    int x; x=getint(d);
este echivalent cu:
    int x; FILE *f;
    f=fdopen(d,"r");
    fscanf(f,"%d",&x);     
(deci functia "getint" va citi caractere cat timp ele se potrivesc cu o
reprezentare externa de intreg zecimal si la sfarsit va returna intregul
respectiv; cand un caracter nu se mai potriveste, indicatorul in fisier este
mutat inapoi cu lseek). In citirea intregului, functia "getint" va ignora
eventualele caractere albe (blank, tab, cap de linie) prealabile. Daca de la
bun inceput intregul nu se poate citi (primul caracter nealb nu poate face 
parte dintr-o reprezentare de intreg sau dupa eventualel caractere albe s-a
intalnit sfarsitul fisierului) functia va returna 0 si va seta adecvat errno.
Scrieti un program ilustrativ pentru aceasta functie.
*/


long get_file_size(int fd) {
    
    int bytes_read;
    long size = 0;
    char t = 0;

    while ((bytes_read = read(fd, &t, 1)) != 0) ++size;

    lseek(fd, 0, SEEK_SET);

    return size + 1;
}

long getint(int fd, long buffer_size) {
    
    char buffer[buffer_size];
    int bytes_read;
    long k = 0;
    char t = 0;
    long count = 0;
    char *ptr;
    while ((bytes_read = read(fd, &t, 1)) != 0) {
        ++count;
        if (!isspace(t)) {
            if (isdigit(t) || (k == 0 && t == '-')) {
                buffer[k] = t;
                ++k;
            } else if (t == '-' && isdigit(buffer[k - 1])) {
                lseek(fd, -1, SEEK_CUR);
                buffer[k] = '\0';
                return strtol(buffer, &ptr, 10);
            } else {
                errno = EIO;
                buffer[k] = '\0';
                lseek(fd, -1, SEEK_CUR); 
                return strtol(buffer, &ptr, 10);
            }
        } else {
            if (k > 0 && isdigit(buffer[k - 1])) {
                buffer[k] = '\0';
                lseek(fd, -1, SEEK_CUR);
                return strtol(buffer, &ptr, 10);    
            } else if (k > 0 && !isdigit(buffer[k - 1])) {
                errno = EIO;
                lseek(fd, -1, SEEK_SET); 
                return 0;
            }
        }
    }
    if (count == 0) {
        errno = ENOMEM;
        return -1;
    }
    if ((k == 0) || (k == 1 && buffer[k - 1] == '-')) {
        errno = EIO;
        return 0;
    }
    buffer[k] = '\0';
    return strtol(buffer, &ptr, 10);
}


int main(int argc, char** argv) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILE\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    long size = get_file_size(fd);
    do {
        long number = getint(fd, size);
        if (errno == EIO) {
            perror("There was an error");
            close(fd);
            return EXIT_FAILURE;
        } else if (errno == ENOMEM) {
            close(fd);
            return EXIT_SUCCESS;
        } else {
            printf("number = %ld\n", number);
        }
    } while (1);

    return EXIT_SUCCESS;


}
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

/*
D2. (4 puncte) Program care primeste ca argument in linia de comanda un
director si calculeaza suma dimensiunilor fisierelor din arborescenta cu
originea in el.
*/

long int recursive_sum_files(char* base_path) {
    
    DIR* directory = opendir(base_path);
    if (directory == NULL) {
        perror(base_path);
        return EXIT_FAILURE;
    }
    struct dirent* entry;
    long int sum = 0;
    while ((entry = readdir(directory))) {

        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            
            char* path;
            if ((path = malloc(sizeof(char) * (strlen(base_path) + strlen(entry->d_name) + 2))) == NULL) {
                fprintf(stderr, "There was a problem allocating the memory\n");
                closedir(directory);
                return EXIT_FAILURE;
            }
            strcpy(path, base_path);
            if (strlen(base_path) != 0 && base_path[strlen(base_path) - 1] != '/') {
                strcat(path, "/");
            }
            strcat(path, entry->d_name);

            if (entry->d_type == DT_DIR) {
                sum += recursive_sum_files(path);
            } // else if (entry->d_type == DT_REG) {
                struct stat st;
                if (stat(path, &st) == -1) perror(path);
                else {
                    printf("%s: %ld bytes\n", path, st.st_size);
                    sum += st.st_size;
                }
            //}
            free(path);
        }
    }
    closedir(directory);

    return sum;

}

int main(int argc, char **argv) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s DIR\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (opendir(argv[1]) == NULL) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    long int sum = recursive_sum_files(argv[1]);
    struct stat st;
    if (stat(argv[1], &st) == -1) perror(argv[1]);
    else {
        sum += st.st_size;
    }
    
    printf("The size of all of the files is: %ld bytes\n", sum);
    
    return EXIT_SUCCESS;

}
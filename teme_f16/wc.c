#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>

int byte_count = 0; 
int line_count = 0; 
int word_count = 0;
int options[3];

int get_options(char *argument) {

    // options = [c: , l: , w: ]
    // example: if options = [0, w, 1] -> the options l and w were provided 
    
    for(int i = 1; i < strlen(argument); ++i) {
        if(argument[i] == 'c') {
            options[0] = 1;
        } else if(argument[i] == 'l') {
            options[1] = 1;
        } else if(argument[i] == 'w') {
            options[2] = 1;
        } else {
            return 1;
        }
    }
    return 0;
}

void all_options() {
    for(int i = 0; i < 3; ++i) {
        options[i] = 1;
    }
}

void from_file(FILE* file) {

    int is_word = 0;
    int c;

    while((c = getc(file)) != EOF) {
        ++byte_count;
        if (c == '\n') {
            ++line_count;
        }
        if(is_word == 0 && !iswspace(c)) {
            ++word_count;
            is_word = 1;
        }
        if(iswspace(c)) {
            is_word = 0;
        }
    }

    
}

void print_values() {
    if(options[0]) {
        fprintf(stdout, "# bytes: %d\n", byte_count);
    }
    if(options[1]) {
        fprintf(stdout, "# lines: %d\n", line_count);
    }
    if(options[2]) {
        fprintf(stdout, "# words: %d\n", word_count);
    }
}


int main(int argc, char **argv){

    if(argc > 3) {
        fprintf(stderr, "You provided too many arguments\nUsage: %s [OPTIONS] [FILE]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (argc == 3) {
        // wc -options file
        if(argv[1][0] == '-') {
            get_options(argv[1]);
        } else {
            fprintf(stderr, "You provided too many files\nUsage: %s [OPTIONS] [FILE]\n", argv[0]);
            return EXIT_FAILURE;
        }
        FILE* f = fopen(argv[2], "r");
        if (f == NULL) {
            perror("There was an error while opening the file your provided.");
            return EXIT_FAILURE;
        }
        from_file(f);
        fclose(f);

    }

    if (argc == 2) {
        // wc -options or wc file
        if (argv[1][0] == '-') {
            // write from stdin
            if(strlen(argv[1]) > 4) {
                fprintf(stderr, "You provided too many options\nUsage: %s [OPTIONS] [FILE]\n", argv[0]);
                return EXIT_FAILURE;
            }
            if (get_options(argv[1])) {
                fprintf(stderr, "The only options available are l, c, and w\nUsage: %s [OPTIONS] [FILE]\n", argv[0]);
                return EXIT_FAILURE;
            }
            from_file(stdin);
        } else {
            all_options();
            FILE* f = fopen(argv[1], "r");
            if (f == NULL) {
                perror("There was an error while opening the file you provided: ");
                return EXIT_FAILURE;
            }
            all_options();
            from_file(f);
            fclose(f);
        }
    }

    if (argc == 1) {
        // wc 
        all_options();
        from_file(stdin);
    }

    print_values(byte_count, line_count, word_count);

    return EXIT_SUCCESS;
}

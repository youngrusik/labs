#include "entrant.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 3){
        perror("Usage: ./to_bin INPUT_FILE OUTPUT_FILE");
        exit(1);
    }
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "wb");

    if (input == NULL){
        perror("Can't open input file!");
        exit(1);
    }
    if (output == NULL){
        perror("Can't create output file!");
        exit(1);
    }

    int chr;
    do {
        chr = fgetc(input);
        if (chr == EOF){
            fprintf(stderr, "Read error!\n");
            return 1;
        }
    } while (chr != '\n');

    Entrant e;
    int counter = 0;
    fseek(output, sizeof(int), SEEK_SET);
    while (fscanf(input, "%[^;]; %[^;]; %c; %d; %d; %d; %d; %d; %d; \n", e.surname, e.initials, &e.gender, &e.school_num, &e.have_medal, &e.math, &e.russian, &e.informatics, &e.essay) == 9) {
        if (fwrite(&e, sizeof(Entrant), 1, output) != 1){
            perror("Write error!");
            exit(1);
        }
        ++counter;
    }
    fseek(output, 0, SEEK_SET);     // pointer to top of file
    if (fwrite(&counter, sizeof(int), 1, output) != 1) {
        perror("Write error!\n");
        exit(1);
    }
    printf("%d lines are written\n", counter);
    fclose(input);
    fclose(output);

    return 0;
}


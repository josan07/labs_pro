#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINES 20
#define MAX_CHARS 256
#define MAX_BLOCKS 50

// criação da estrutura BlockDB que age como base de dados

typedef struct {
    char **keywords;
    int nr_keywords;
    int alloc_space_keywords;

    char **answers;
    int nr_answers;
    int alloc_space_answers;

    int rr_counter;

} BlockDB;

void read_file(char*, char*, char*, char*, BlockDB**, int*);
void format_input(char*);
void search_database(char*, BlockDB normal[MAX_BLOCKS], int, FILE*, FILE*, char);
char* clean_boundaries(char*, char*);
void help();
void fprintf_2(FILE*, FILE*, char*);
void conjugate(char*, char**, char**, int, char*, char);

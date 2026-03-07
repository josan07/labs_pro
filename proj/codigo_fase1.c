#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 20
#define MAX_CHARS 256
#define MAX_BLOCKS 50

// prototypes
void read_database(char*, char*, char*, char*, BlockDB normal[MAX_BLOCKS], BlockDB*, int*);
void format_input(char*);

typedef struct {
    char keywords[MAX_LINES][MAX_CHARS];
    int nr_keywords;

    char answers[MAX_LINES][MAX_CHARS];
    int nr_answers;

    int rr_counter;
} BlockDB;

typedef struct {
    char answers[MAX_LINES][MAX_CHARS];
    int nr_answers;

    int rr_counter;
} NoKeyDB;

int main() {
    char* greeting;
    char* repetition;
    char* farewell;
    BlockDB normal[MAX_BLOCKS];
    BlockDB* no_key;
    int* nr_blocks;

    read_database("eliza.dat", greeting, repetition, farewell, normal, no_key, nr_blocks);

    // stdin

    char former_line[MAX_CHARS];
    char line[MAX_CHARS];

    printf("%s", greeting);
    while (1) {
        fgets(line, MAX_CHARS, stdin);
        line[strcspn(line, "\n")] = '\0';

        format_input(line);

        if (strcmp("\n", line) == 0)
            continue;

        else if (strcmp("BYE", line) == 0) {
            printf("%s", farewell);
            break;
        } else if (strcmp(former_line, line) == 0) {
            printf("%s", repetition);
            continue;
        } else {
            pass;  // search database
        }

        strcpy(former_line, line);
    }
}

void read_database(char* filename, char* greeting, char* repetition, char* farewell,
                   BlockDB normal[MAX_BLOCKS], BlockDB* no_key, int* nr_blocos) {
    FILE* fptr;
    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Error, could not open file\n");
        return;
    }

    memset(normal, 0, sizeof(BlockDB) * MAX_BLOCKS);

    int state = 0;  // 0 para keywords e 1 para possiveis respostas
    int block_counter = 0;

    char buffer[MAX_CHARS];

    while (fgets(buffer, sizeof(buffer), fptr)) {
        // elimina enters
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // ignora linha vazia
        if (strlen(buffer) == 0) continue;

        // converte para maiusculas
        for (int i = 0; buffer[i]; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        if (strcmp(buffer, "!") == 0) {
            state = 1;
            continue;
        }
        if (strcmp(buffer, ".") == 0) {
            state = 0;
            block_counter++;
            continue;
        }

        if (block_counter == 0 && state) {
            strcpy(greeting, buffer);
        } else if (block_counter == 1 && state) {
            strcpy(repetition, buffer);
        } else if (block_counter == 2 && state) {
            strcpy(farewell, buffer);
        } else if (block_counter >= 3) {
            BlockDB* current_db = &normal[block_counter - 3];
            if (!state) {  // armazenar keywords
                strcpy(current_db->keywords[current_db->nr_keywords++], buffer);
            } else {  // armazenar answers
                strcpy(current_db->answers[current_db->nr_answers++], buffer);
            }
        }
    }

    fclose(fptr);

    if (block_counter > 3) {
        NoKeyDB no_key.answers = normal[block_counter - 4];
        *no_key = normal[block_counter - 4];
    }
}

void format_input(char* line) {
    // deixar apenas caracteres alnum/espaços; converter letras para maiusculas

    char* read = line;
    char* write = line;

    while (*read != '\0') {
        if (isalnum(*read) || isspace(*read)) {
            *write = toupper(*read);
            write++;
        }
        read++;
    }
    *write = '\0';
}

void search_database(char* line, BlockDB normal[MAX_BLOCKS], BlockDB* no_key, int* nr_blocks) {
    char* keyword_to_check;
    int keyword_found;
    for (int i = 0; i < (nr_blocks - 4); i++) {  // iterar  nos blocos
        int k = normal[i].nr_keywords;
        int a = normal[i].nr_answers;

        for (int j = 0; j < k; j++) {
            keyword_to_check = normal[i].keywords[j];

            if (strstr(line, keyword_to_check) != NULL) {
                printf("%s", normal[i].answers[normal[i].rr_counter]);
                normal[i].rr_counter = (normal[i].rr_counter + 1) % normal[i].nr_answers;
                keyword_found = 1;
            }
        }
        if (keyword_found) break;
    }
    if (!keyword_found) {
    }
}

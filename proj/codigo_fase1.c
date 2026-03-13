#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 20
#define MAX_CHARS 256
#define MAX_BLOCKS 50

// criação da estrutura BlockDB que age como base de dados

typedef struct {
    char keywords[MAX_LINES][MAX_CHARS];
    int nr_keywords;

    char answers[MAX_LINES][MAX_CHARS];
    int nr_answers;

    int rr_counter;
} BlockDB;

// declarações das funções usadas no programa (PROTOTYPES)

void read_file(char* filename, char* greeting, char* repetition, char* farewell, BlockDB normal[MAX_BLOCKS], int* nr_blocos);
void format_input(char* line);
void search_database(char* line, BlockDB normal[MAX_BLOCKS], int nr_blocks);
int find_in_line(char* line, char* keyword_to_check);

int main() {
    char greeting[MAX_CHARS];
    char repetition[MAX_CHARS];
    char farewell[MAX_CHARS];
    BlockDB normal[MAX_BLOCKS];
    int nr_blocks = 0;

    // chamar a função read_file, que regista a base de dados após ler o ficheiro
    read_file("eliza.dat", greeting, repetition, farewell, normal, &nr_blocks);

    char former_line[MAX_CHARS] = "";
    // registo da ultima entrada, util pra chamar o repetition
    char line[MAX_CHARS];

    // antes de tudo, usar greeting para saudar o utilizador
    printf("%s\n", greeting);

    // ciclo que recebe como inputs stdin e os regista de cada vez como line, terminando implicitamente no EOF
    while (fgets(line, sizeof(line), stdin)) {
        // cortar a linha no enter
        line[strcspn(line, "\r\n")] = '\0';

        // formatar a linha
        format_input(line);

        // ignorar se o user deu enter sem escrever nada
        if (strlen(line) == 0) continue;

        // ignorar linhas apenas com espaços (input tipo '   ')
        int spaces_only = 1;
        for (int i = 0; line[i] != 0; i++) {
            if (!isspace(line[i])) {
                spaces_only = 0;
                break;
            }
        }

        if (spaces_only) continue;

        // mostrar mensagem de despedida se o user digitar especificamente 'BYE'
        if (strcmp("BYE", line) == 0) {
            printf("%s\n", farewell);
            break;
        }

        // comparar linha atual com linha imediatamente anterior: se forem iguais mostrar mensagem de repetição
        if (strcmp(former_line, line) == 0) {
            printf("%s\n", repetition);
            continue;
        }

        // atualizar valor da ultima linha, para poder comparar a proxima linha com esta
        strcpy(former_line, line);

        // invocar função search_database para dar print à resposta (Stdout)
        search_database(line, normal, nr_blocks);
    }
    return EXIT_SUCCESS;
}

void read_file(char* filename, char* greeting, char* repetition,
               char* farewell, BlockDB normal[MAX_BLOCKS], int* nr_blocks) {
    // abrir o file pointer
    FILE* fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Error, could not open file\n");
        exit(EXIT_FAILURE);
        ;
    }

    // zerar a memória da base de dados
    memset(normal, 0, sizeof(BlockDB) * MAX_BLOCKS);

    // maquina de estados: 0 para keywords e 1 para possiveis respostas
    int state = 0;
    // contador de blocos, especialmente util quando forem necessarios os ciclos for
    int block_counter = 0;

    // criacao de um buffer temporário usado para armazenar a linha do ficheiro nas entradas bases de dados
    char buffer[MAX_CHARS];

    while (fgets(buffer, sizeof(buffer), fptr)) {
        // buffer armazena 1 linha de cada vez

        // elimina enters
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // ignora linha vazia
        if (strlen(buffer) == 0) continue;

        // converte para maiusculas
        for (int i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        // maquina de estados e contador de blocos: '!' e '.' alternam o estado 0-1; '.' tambem conta blocos
        if (strcmp(buffer, "!") == 0) {
            state = 1;
            continue;
        }
        if (strcmp(buffer, ".") == 0) {
            state = 0;
            block_counter++;
            continue;
        }

        /*  ARMAZENAR DADOS
            1.º bloco: armazenar no vector greeting
            2.º bloco: armazenar no vector repetition
            3.º bloco: armazenar no vector farewell
        */

        if (block_counter == 0 && state) {
            strcpy(greeting, buffer);
        } else if (block_counter == 1 && state) {
            strcpy(repetition, buffer);
        } else if (block_counter == 2 && state) {
            strcpy(farewell, buffer);
        } else if (block_counter >= 3) {
            // pointer temporário que aponta para entradas especificas da base de dados
            BlockDB* current_db = &normal[block_counter - 3];
            if (!state) {  // armazenar keywords
                strcpy(current_db->keywords[current_db->nr_keywords++], buffer);
            } else {  // armazenar answers
                strcpy(current_db->answers[current_db->nr_answers++], buffer);
            }
        }
    }

    /*nr de blocos da base de dados:
    subtraimos 3 porque os 3 primeiros blocos
    nao constam da base de dados*/
    *nr_blocks = block_counter - 3;

    // fechar filepointer
    fclose(fptr);
}

void format_input(char* line) {
    // deixar apenas caracteres alnum/espaços; converter letras para maiusculas

    char* read = line;
    char* write = line;

    // ignorar os espaços do inicio do input
    while (*read != '\0' && isspace(*read)) read++;

    while (*read != '\0') {
        if (isalnum(*read) || isspace(*read)) {
            *write = toupper(*read);
            write++;
        }
        read++;
    }
    *write = '\0';

    // write > line pra prevenir procurar na posicao -1 se por exemplo input='   ' e todos os espaços ja tiverem sido suprimidos
    while (write > line && isspace(*(write - 1))) {
        write--;
        *write = 0;
    }
}

void search_database(char* line, BlockDB normal[MAX_BLOCKS], int nr_blocks) {
    char* keyword_to_check;
    int keyword_found = 0;
    int num_keyword_blocks = nr_blocks - 1;
    for (int i = 0; i < num_keyword_blocks; i++) {  // iterar  nos blocos
        int k = normal[i].nr_keywords;

        // k para iterar ao longo das keywords de  cada bloco
        for (int j = 0; j < k; j++) {
            keyword_to_check = normal[i].keywords[j];
            // checkar a keyword índice j
            if ((strstr(line, keyword_to_check) != NULL) && find_in_line(line, keyword_to_check)) {  // encontrar keyword na frase
                printf("%s\n", normal[i].answers[normal[i].rr_counter]);
                normal[i].rr_counter = (normal[i].rr_counter + 1) % normal[i].nr_answers;
                keyword_found = 1;
                return;
            }
        }
    }
    if (!keyword_found) {  // nao encontrou keyword!
        int nkf_pos = nr_blocks - 1;
        printf("%s\n", normal[nkf_pos].answers[normal[nkf_pos].rr_counter]);
        normal[nkf_pos].rr_counter = (normal[nkf_pos].rr_counter + 1) % normal[nkf_pos].nr_answers;
        return;
    }
}

int find_in_line(char* line, char* keyword_to_check) {
    /*esta funcao certifica-se de que a keyword corresponde efectivamente a uma palavra
    da frase de input. Garantir que por exemplo "nothing" não desperta "HI".
    Especificamos as boundaries esquerda (inicio de input ou espaço) e direita ('\0' ou espaço)*/
    char* pos_1st_letter = strstr(line, keyword_to_check);

    while (pos_1st_letter != NULL) {
        char* pos_last_letter = pos_1st_letter + strlen(keyword_to_check);
        int clean_left = (pos_1st_letter == line) || (!isalnum(*(pos_1st_letter - 1)));
        int clean_right = ((*pos_last_letter) == '\0') || (!isalnum(*(pos_last_letter)));

        if (clean_left && clean_right) return 1;

        pos_1st_letter = strstr(pos_1st_letter + 1, keyword_to_check);
    }

    return 0;
}

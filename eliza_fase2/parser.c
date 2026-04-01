#include "eliza.h"

/*
 * A função format_input formata a string de input do utilizador.
 * Recebe como argumento o ponteiro para a string de input a ser formatada.
 * Converte caracteres alfanuméricos para maiúsculas e elimina caracteres que
 * não sejam dígitos, letras ou espaços. Remove espaços à esquerda e à direita
 * e espaços a mais entre palavras.
 */

void format_input(char* line) {
    char* read = line;    // ponteiro de leitura
    char* write = line;   // ponteiro de escrita
    int found_space = 0;  // flag que controla nr de espaços seguidos

    while (*read != '\0' && isspace(*read)) read++;  // ignorar espaços iniciais

    while (*read != '\0') {    // percorrer string até ao fim
        if (isalnum(*read)) {  // guardar alfanuméricos, converter para maiúsculas
            *write = toupper(*read);
            write++;
            found_space = 0;
        } else if (isspace(*read)) {
            // guardar espaços de acordo com a flag found_space
            if (!found_space) {
                *write = *read;
                write++;
            }
            found_space = 1;
        }
        read++;
    }
    *write = '\0';

    while (write > line && isspace(*(write - 1))) {  // remover espaços da direita
        write--;
        *write = 0;
    }
}

/**
 * A função clean_boundaries procura uma palavra-chave garantindo que esta
 * é uma palavra isolada. Utiliza o strstr para encontrar a ocorrência,
 * mas verifica se os caracteres imediatamente à esquerda e à direita
 * não são alfanuméricos. Útil para evitar  * "falsos positivos",
 * como ler "HI" em "NOTHING".  * Depende de dois ponteiros,
 * passados como argumentos:
 * > line, a linha de input formatado do user
 * > keyword, a keyword a procurar na linha
 */

char* clean_boundaries(char* line, char* keyword) {
    char* pos = strstr(line, keyword);  // ponteiro para o início da keyword no input
    while (pos != NULL) {
        char* end = pos + strlen(keyword);  // ponteiro para o fim da keyword no input

        /**
         * > clean_left ativo a 1 se a keyword se encontrar logo no início de line
         * ou se o caracter imediatamente à esquerda não for alnum
         * > clean_right ativo a 1 se a keyword se encontrar no fim de line ou se
         * o caracter imediatamente à direita nao for alnum
         */

        int clean_left = (pos == line) || (!isalnum(*(pos - 1)));
        int clean_right = ((*end) == '\0') || (!isalnum(*end));


        // Se as boundaries estiverem limpas, é uma palavra isolada e devolve a posição
        if (clean_left && clean_right) return pos;

        // Caso contrário, continua a procurar na restante string
        pos = strstr(pos + 1, keyword);
    }

    // return NULL se não encontrou a keyword isoladamente
    return NULL;
}
